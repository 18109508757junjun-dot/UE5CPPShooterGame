// Copyright Epic Games, Inc. All Rights Reserved.

#include "ShooterGameCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "ShooterGame.h"
#include "ShooterGamePlayerController.h"
#include "CollectableItem.h"
#include "Lock.h"



AShooterGameCharacter::AShooterGameCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 500.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;
	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character)
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

void AShooterGameCharacter::BeginPlay()
{
	Super::BeginPlay();

	Health = MaxHealth;
	UpdatHud();//防止血条一开始不是满的
	OnTakeAnyDamage.AddDynamic(this, &AShooterGameCharacter::OnTakeDamage);//绑定一个事件，当角色受到任何伤害时，都会调用OnTakeDamage函数来处理伤害逻辑。

	GetMesh()->HideBoneByName(TEXT("weapon_r"), EPhysBodyOp::PBO_None);//隐藏角色骨骼中的weapon_r骨骼，这样我们就不会看到角色手里拿着枪了
	Gun = GetWorld()->SpawnActor<AGun>(GunClass);//生成一个Gun的实例，并将其赋值给Gun变量 ps，Gun就是指向我们的枪的指针
	if (Gun)
	{
		Gun->SetOwner(this);//设置枪的拥有者为当前角色。
		Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("weapon_rSocket"));//将枪附加到角色的Mesh组件上的WeaponSocket插槽位置，这样枪就会跟随角色的动画和位置移动。
		Gun->GunOwner = GetController();//设置枪的GunOwner变量为当前角色的Controller，这样枪就知道它是被哪个角色控制的了。
	}

}

void AShooterGameCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {

		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AShooterGameCharacter::Move);
		EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &AShooterGameCharacter::Look);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AShooterGameCharacter::Look);

		//Shooting
		EnhancedInputComponent->BindAction(ShootingAction, ETriggerEvent::Started, this, &AShooterGameCharacter::Shoot);//玩家执行设计动作，调用函数shoot

		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &AShooterGameCharacter::Interact);
	}
	else
	{
		UE_LOG(LogShooterGame, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AShooterGameCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	// route the input
	DoMove(MovementVector.X, MovementVector.Y);
}

void AShooterGameCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	// route the input
	DoLook(LookAxisVector.X, LookAxisVector.Y);
}



void AShooterGameCharacter::DoMove(float Right, float Forward)
{
	if (GetController() != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = GetController()->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement
		AddMovementInput(ForwardDirection, Forward);
		AddMovementInput(RightDirection, Right);
	}
}

void AShooterGameCharacter::DoLook(float Yaw, float Pitch)
{
	if (GetController() != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(Yaw);
		AddControllerPitchInput(Pitch);
	}
}

void AShooterGameCharacter::DoJumpStart()
{
	// signal the character to jump
	Jump();
}

void AShooterGameCharacter::DoJumpEnd()
{
	// signal the character to stop jumping
	StopJumping();
}
//射击动作实现
void AShooterGameCharacter::Shoot()
{
	//UE_LOG(LogTemp, Display, TEXT("Shooting"));
	if (!IsAlive || !Gun)
	{
		return;
	}
	Gun->PullTrigger();
}
//机关交互
void AShooterGameCharacter::Interact()
{
	//UE_LOG(LogTemp,Display,TEXT("Interact"));
	FVector Start = FollowCamera->GetComponentLocation();//获取摄像机位置作为起始点
	FVector End = Start + (FollowCamera->GetForwardVector() * MaxInteractDistance);//终点位置=起始点+（摄像机前向量*最大交互距离）



	FCollisionShape InteractSphere = FCollisionShape::MakeSphere(InteractSphereRadius);//创建一个球形碰撞体，参数是半径


	FHitResult HitResult;//创建一个碰撞结果结构体，用于存储碰撞检测的结果
	//这个函数是进行碰撞检测的函数，使用球形碰撞体从起始点到终点进行检测，检测碰撞通道为ECC_GameTraceChannel2（自定义通道），将结果存储在HitResult中
	bool HasHit = GetWorld()->SweepSingleByChannel(
		HitResult,
		Start, End,
		FQuat::Identity,
		ECC_GameTraceChannel2,
		InteractSphere);//该函数参数从左到右依次是碰撞结果，开始位置，结束位置，旋转（这里使用默认值），碰撞通道(自定义通道)，碰撞形状

	if (HasHit)
	{
		AActor* HitActor = HitResult.GetActor();//获取被碰撞到的Actor
		//UE_LOG(LogTemp, Display, TEXT("Hit Actor: %s"), *HitActor->GetActorNameOrLabel());//输出被碰撞到的Actor的名字

		if (HitActor->ActorHasTag("CollectableItem"))
		{
			//碰撞的actor是CollectableItem标签

			ACollectableItem* CollectableItem = Cast<ACollectableItem>(HitActor);//将被碰撞到的Actor转换为ACollectableItem类型,这样就可以访问ACollectableItem类中的属性和方法了
			if (CollectableItem)
			{
				//UE_LOG(LogTemp, Display, TEXT("Hit a Collectable Item with name %s!"),*CollectableItem->ItemName);
				ItemList.Add(CollectableItem->ItemName);//将被碰撞到的物品的名字添加到角色的物品列表中
				CollectableItem->Destroy();//销毁被碰撞到的物品
			}
		}

		else if (HitActor->ActorHasTag("Lock"))
		{
			//碰撞的actor是Lock标签

			ALock* LockActor = Cast<ALock>(HitActor);
			if (LockActor)
			{
				//UE_LOG(LogTemp, Display, TEXT("Hit a Lock with name %s!"),*LockActor->KeyItemName);
                if (!LockActor->GetIsKeyPlaced()) //检查机关上是否放置钥匙，改为使用LockActor的GetIsKeyPlaced()
				{
					int32 ItemsRemoved = ItemList.RemoveSingle(LockActor->KeyItemName);//从物品列表中移除一个与锁的钥匙名字相同的物品
					if (ItemsRemoved)
					{
						LockActor->SetIsKeyPlaced(true);
					}
					else
					{
						UE_LOG(LogTemp, Display, TEXT("The lock is not in inventory!"));
					}
				}
				else//机关上有钥匙
				{
					ItemList.Add(LockActor->KeyItemName);
					LockActor->SetIsKeyPlaced(false);
				}

			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("No Actor Hit"));//如果没有碰撞到任何Actor，输出没有碰撞到Actor
	}
}

//更新血量
void AShooterGameCharacter::UpdatHud()
{
	AShooterGamePlayerController* PlayerController = Cast<AShooterGamePlayerController>(GetController());//获取玩家控制器
	if (PlayerController && PlayerController->HudWidget)
	{
		float NewPercent = Health / MaxHealth;
		if (NewPercent < 0.0f)
		{
			NewPercent = 0.0f;
		}

		PlayerController->HudWidget->SetHealthBarPercent(NewPercent);//通过SetHealthBarPercent控制血条更新
	}

}
//角色受伤逻辑实现
void AShooterGameCharacter::OnTakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{

	if (IsAlive)
	{
		//UE_LOG(LogTemp, Display, TEXT("Damage taken: %f"), Damage);
		Health -= Damage;
		UpdatHud();
		if (Health <= 0)
		{
			IsAlive = false;
			Health = 0;
			GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

			// 防止死亡瞬间仍触发射击/使用失效 Controller
			if (Gun)
			{
				Gun->GunOwner = nullptr;
			}

			// 不要在这里立即 Detach Controller：输入/武器/HUD 可能仍在同一帧访问 Controller。
			// 练习项目里先禁用输入和移动，避免死亡后继续触发行为。
			if (AController* CurrentController = GetController())
			{
				DisableInput(Cast<APlayerController>(CurrentController));
			}
			GetCharacterMovement()->DisableMovement();
			//UE_LOG(LogTemp, Display, TEXT("Character died: %s"), *GetActorNameOrLabel());
		}



	}
}
