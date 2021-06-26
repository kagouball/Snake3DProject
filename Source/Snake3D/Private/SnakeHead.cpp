// Fill out your copyright notice in the Description page of Project Settings.


#include "SnakeHead.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Runtime/Engine/Classes/GameFramework/InputSettings.h"
#include "MyPawnMovementComponent.h"
#include "MovementTag.h"
#include "PlayField.h"
#include "PlayerHUD.h"
#include "SnakePlayer.h"

// Sets default values
ASnakeHead::ASnakeHead()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//Values
	radius = 20.f;
	PitchValue = 0.f;
	YawValue = 0.f;
	RollValue = 0.f;
	isCameraMoving = false;
	makeAngle = false;
	personMode = TPS;
	// Our root component will be a sphere that reacts to physics
	USphereComponent* SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("RootComponent"));
	RootComponent = SphereComponent;
	SphereComponent->InitSphereRadius(radius);
	//SphereComponent->SetGenerateOverlapEvents(true);
	//SphereComponent->SetCollisionProfileName(FName("Pawn"));
	// Create and position a mesh component so we can see where our sphere is
	UStaticMeshComponent* SphereVisual = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualRepresentation"));
	SphereVisual->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereVisualAsset(TEXT("/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere"));
	if (SphereVisualAsset.Succeeded())
	{
		SphereVisual->SetStaticMesh(SphereVisualAsset.Object);
		SphereVisual->SetRelativeLocation(FVector(0.0f, 0.0f, -radius));
		SphereVisual->SetWorldScale3D(FVector(radius / 50));
		//SphereVisual->SetGenerateOverlapEvents(false);
		//SphereVisual->SetCollisionProfileName(FName("NoCollision"));
	}
	// Use a spring arm to give the camera smooth, natural-feeling motion.
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraAttachmentArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->SetRelativeRotation(FRotator(-20.f, 0.f, 0.f));
	SpringArm->SetRelativeLocation(FVector(0.f, 0.f, 40.f));
	SpringArm->TargetArmLength = 200.0f;
	SpringArm->bEnableCameraLag = true;
	//SpringArm->bDoCollisionTest = false;
	SpringArm->CameraLagSpeed = 3.0f;
	// Create a camera and attach to our spring arm
	UCameraComponent* Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("ActualCamera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	// Take control of the default player
	AutoPossessPlayer = EAutoReceiveInput::Player0;
	// Create an instance of our movement component, and tell it to update the root.
	OurMovementComponent = CreateDefaultSubobject<UMyPawnMovementComponent>(TEXT("CustomMovementComponent"));
	OurMovementComponent->UpdatedComponent = RootComponent;
	//listener
	//OnActorBeginOverlap.AddDynamic(this, &ASnakeHead::OnOverlapBegin);
	sphereComponent = SphereComponent;
	//SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ASnakeHead::OnOverlap);
	//OnActorBeginOverlap.Clear();
}


// Called when the game starts or when spawned
void ASnakeHead::BeginPlay()
{
	Super::BeginPlay();
	//sphereComponent->SetGenerateOverlapEvents(true);
	sphereComponent->SetCollisionProfileName(FName("Pawn"));
	//sphereComponent->OnComponentBeginOverlap.Clear();
	sphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ASnakeHead::OnOverlap);
}

// Called every frame
void ASnakeHead::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	MoveForward(player->GetPlayerSpeed());
	UpdateRotation();
}

// Called to bind functionality to input
void ASnakeHead::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	//Axis
	PlayerInputComponent->BindAxis("TurnCameraRight", this, &ASnakeHead::TurnRightCamera);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASnakeHead::Turn);
	PlayerInputComponent->BindAxis("MoveTop", this, &ASnakeHead::MoveTop);
	PlayerInputComponent->BindAxis("Rotate", this, &ASnakeHead::Rotate);
	//Button
	PlayerInputComponent->BindAction("SpawnObject", IE_Pressed, player, &ASnakePlayer::AddPiece);
	PlayerInputComponent->BindAction("SwitchPersonView", IE_Pressed, this, &ASnakeHead::SwitchCameraPersonView);
	//Angle
	PlayerInputComponent->BindAction("AngleRight", IE_Pressed, this, &ASnakeHead::AngleRight);
	PlayerInputComponent->BindAction("AngleLeft", IE_Pressed, this, &ASnakeHead::AngleLeft);
	PlayerInputComponent->BindAction("AngleTop", IE_Pressed, this, &ASnakeHead::AngleTop);
	PlayerInputComponent->BindAction("AngleBottom", IE_Pressed, this, &ASnakeHead::AngleBottom);
}

//Movement

UPawnMovementComponent* ASnakeHead::GetMovementComponent() const
{
	return OurMovementComponent;
}

void ASnakeHead::MoveForward(float AxisValue)
{
	float FrameRate = Tools::GetSafeFramerate(GetWorld()->GetDeltaSeconds());
	FVector loc = GetActorLocation();
	loc += GetActorForwardVector() * ((AxisValue * 100.f) / FrameRate);	//To get a constant value depending of framerate
	SetActorLocation(loc);
}

void ASnakeHead::Turn(float AxisValue)
{
	if (AxisValue != 0.f) {
		YawValue += AxisValue;
	}
}

void ASnakeHead::MoveTop(float AxisValue)
{
	if (AxisValue != 0.f) {//permet de ne pas update à tous les tick
		PitchValue += AxisValue;
	}
}

void ASnakeHead::Rotate(float AxisValue)
{
	if (AxisValue != 0.f) {
		RollValue += AxisValue;
	}
}

void ASnakeHead::AngleTop()
{
	makeAngle = true;
	angle = Angle::GetRightAngleTop();
}

void ASnakeHead::AngleBottom()
{
	makeAngle = true;
	angle = Angle::GetRightAngleBottom();
}

void ASnakeHead::AngleRight()
{
	makeAngle = true;
	angle = Angle::GetRightAngleRight();
}

void ASnakeHead::AngleLeft()
{
	makeAngle = true;
	angle = Angle::GetRightAngleLeft();
}

void ASnakeHead::UpdateRotation() {
	if (YawValue == 0.f && PitchValue == 0.f && RollValue == 0.f && !makeAngle) {
		return;	//early return
	}

	FQuat QuatRotation;
	if (!makeAngle)
	{
		float FrameRate = Tools::GetSafeFramerate(GetWorld()->GetDeltaSeconds());
		QuatRotation = FQuat(FRotator(PitchValue, YawValue, RollValue) * (100.f / FrameRate));
	}
	else
	{
		//rotation 90 cst
		QuatRotation = angle;
		makeAngle = false;
	}

	AddActorLocalRotation(QuatRotation, false, 0, ETeleportType::None);
	//Spawn tag
	player->HeadMove();
	YawValue = 0.f;
	RollValue = 0.f;
	PitchValue = 0.f;
}

//Camera movement
void ASnakeHead::TurnRightCamera(float AxisValue) {
	if (AxisValue != 0.f) {
		float camYaw = SpringArm->GetRelativeRotation().Yaw;
		if (FMath::Abs(camYaw + AxisValue) < 90.f) //90: angle max
		{ 
			float framerate = Tools::GetSafeFramerate(GetWorld()->GetDeltaSeconds());
			SpringArm->SetRelativeRotation(FRotator(SpringArm->GetRelativeRotation().Pitch, camYaw + (AxisValue * 100.f/framerate), 0.f));
		}
	}
	else {
		UpdateCamera();
	}
}

void ASnakeHead::TurnUpCamera(float AxisValue) {

}

void ASnakeHead::UpdateCamera() {
	float camYaw = SpringArm->GetRelativeRotation().Yaw;
	if (camYaw != 0.f) {
		float sign = UKismetMathLibrary::SignOfFloat(camYaw);
		float newRot = FMath::Clamp(FMath::Abs(camYaw) -1.f, 0.f,90.f);
		SpringArm->SetRelativeRotation(FRotator(SpringArm->GetRelativeRotation().Pitch, newRot*sign, SpringArm->GetRelativeRotation().Roll));
	}
}

void ASnakeHead::SwitchCameraPersonView() {
	if (personMode == TPS) {
		personMode = FPS;
	}
	else {
		personMode = TPS;
	}
	SetCameraPersonView(personMode);
}

void ASnakeHead::SetCameraPersonView(CameraPersonModes mode) {
	if (mode == TPS) 
	{
		SpringArm->SetRelativeRotation(FRotator(-20.f, 0.f, 0.f));
		SpringArm->SetRelativeLocation(FVector(0.f, 0.f, 40.f));
		SpringArm->TargetArmLength = 200.0f;
		SpringArm->bEnableCameraLag = true;
		SpringArm->CameraLagSpeed = 3.0f;
	}
	else 
	{
		SpringArm->SetRelativeRotation(FRotator(0.f, 0.f, 0.f));
		SpringArm->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
		SpringArm->TargetArmLength = 0;
		SpringArm->bEnableCameraLag = false;
	}
}

//Others

void ASnakeHead::OnOverlapBegin2(AActor* OverlappedActor, AActor* OtherActor)
{
	if (OtherActor->ActorHasTag(FName("Snake.Piece"))) 
	{
		ASnakePiece* piece = dynamic_cast<ASnakePiece*>(OtherActor);
		player->HitPiece(piece);
	}
	else if (OtherActor->ActorHasTag(FName("Food"))) 
	{
		AFood* food = dynamic_cast<AFood*>(OtherActor);
		
		UE_LOG(LogTemp, Warning, TEXT("actor collideur : %s"), *OverlappedActor->GetFName().ToString());
		UE_LOG(LogTemp, Warning, TEXT("actor collidé : %s"), *OtherActor->GetActorLabel());
		UE_LOG(LogTemp, Warning, TEXT("time : %f"),GetWorld()->GetTimeSeconds());
		UE_LOG(LogTemp, Warning, TEXT("food collision : %s"),*food->GetActorLocation().ToString());

		HitFood(food);
	}
}

void ASnakeHead::OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool FromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag(FName("Snake.Piece")))
	{
		ASnakePiece* piece = dynamic_cast<ASnakePiece*>(OtherActor);
		player->HitPiece(piece);
	}
	else if (OtherActor->ActorHasTag(FName("Food")))
	{
		AFood* food = dynamic_cast<AFood*>(OtherActor);

		UE_LOG(LogTemp, Warning, TEXT("comp collideur : %s"), *OverlappedComp->GetFName().ToString());
		UE_LOG(LogTemp, Warning, TEXT("actor collidé : %s"), *OtherActor->GetActorLabel());
		UE_LOG(LogTemp, Warning, TEXT("time : %f"), GetWorld()->GetTimeSeconds());
		UE_LOG(LogTemp, Warning, TEXT("food collision : %s"), *food->GetActorLocation().ToString());

		HitFood(food);
	}
}

void ASnakeHead::Collapse(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool FromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Collision"));
	if (OtherActor->ActorHasTag(FName("Food")))
	{
		UE_LOG(LogTemp, Warning, TEXT("Collision Food"));
		if (OtherComp->ComponentHasTag(FName("FoodVisual"))) {
			UE_LOG(LogTemp, Warning, TEXT("Collision Food Visual"));
		}
		else if (OtherComp->ComponentHasTag(FName("FoodSphere"))) {
			UE_LOG(LogTemp, Warning, TEXT("Collision Food Sphere"));
		}
		
	}
}

void ASnakeHead::HitFood(AFood* food) 
{

	player->EatFood(food);
}

void ASnakeHead::Kill() 
{
	UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
}

void ASnakeHead::SetPlayer(ASnakePlayer* p)
{
	player = p;
}







