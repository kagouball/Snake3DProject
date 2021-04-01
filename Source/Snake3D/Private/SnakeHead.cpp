// Fill out your copyright notice in the Description page of Project Settings.


#include "SnakeHead.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/SphereComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Runtime/Engine/Classes/GameFramework/InputSettings.h"

#include "MyPawnMovementComponent.h"
#include "MovementTag.h"
#include "PlayField.h"
#include "PlayerHUD.h"

// Sets default values
ASnakeHead::ASnakeHead()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//Values
	PitchValue = 0.f;
	YawValue = 0.f;
	RollValue = 0.f;
	radius = 20;
	Score = 0;
	isCameraMoving = false;
	personMode = TPS;
	if (!Field) {
		Field = NULL;
	}
	// Our root component will be a sphere that reacts to physics
	USphereComponent* SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("RootComponent"));
	RootComponent = SphereComponent;
	SphereComponent->InitSphereRadius(radius);
	SphereComponent->SetCollisionProfileName(TEXT("Pawn"));
	// Create and position a mesh component so we can see where our sphere is
	UStaticMeshComponent* SphereVisual = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualRepresentation"));
	SphereVisual->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereVisualAsset(TEXT("/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere"));
	if (SphereVisualAsset.Succeeded())
	{
		SphereVisual->SetStaticMesh(SphereVisualAsset.Object);
		SphereVisual->SetRelativeLocation(FVector(0.0f, 0.0f, -radius));
		SphereVisual->SetWorldScale3D(FVector(radius/50));
	}
	// Use a spring arm to give the camera smooth, natural-feeling motion.
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraAttachmentArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->SetRelativeRotation(FRotator(-20.f, 0.f, 0.f));
	SpringArm->SetRelativeLocation(FVector(0.f, 0.f, 40.f));
	SpringArm->TargetArmLength = 200.0f;
	SpringArm->bEnableCameraLag = true;
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
	OnActorBeginOverlap.AddDynamic(this, &ASnakeHead::OnOverlapBegin);
}

// Called when the game starts or when spawned
void ASnakeHead::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ASnakeHead::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	MoveForward(SPEED);
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
	PlayerInputComponent->BindAction("SpawnObject", IE_Pressed, this, &ASnakeHead::AddPiece);
	PlayerInputComponent->BindAction("SwitchPersonView", IE_Pressed, this, &ASnakeHead::SwitchCameraPersonView);
	//Angle
	PlayerInputComponent->BindAction("AngleRight", IE_Pressed, this, &ASnakeHead::AngleRight);
	PlayerInputComponent->BindAction("AngleLeft", IE_Pressed, this, &ASnakeHead::AngleLeft);
	PlayerInputComponent->BindAction("AngleTop", IE_Pressed, this, &ASnakeHead::AngleTop);
	PlayerInputComponent->BindAction("AngleBottom", IE_Pressed, this, &ASnakeHead::AngleBottom);
}

void ASnakeHead::AddPiece()
{
	float distance = 2.f*radius+1.f;
	FVector actualPos;
	FVector vec;
	FRotator rotation;
	if (LastPiece) 
	{
		actualPos = LastPiece->GetActorLocation();
		vec = LastPiece->GetActorForwardVector();
		rotation = LastPiece->GetActorRotation();
	}
	else 
	{
		actualPos = GetActorLocation();
		vec = GetActorForwardVector();
		rotation = GetActorRotation();
	}
	FVector newPos = actualPos - (vec*distance);
	SpawnPiece(newPos, rotation);
}

//Movement

UPawnMovementComponent* ASnakeHead::GetMovementComponent() const
{
	return OurMovementComponent;
}

void ASnakeHead::MoveForward(float AxisValue)
{
	float deltaSec = GetWorld()->GetDeltaSeconds();
	if (deltaSec == 0.f) 
	{
		return; //early return
	}
	float FrameRate = 1.f / deltaSec;
	if (FrameRate == 0.f)
	{
		return; //early return
	}

	FVector loc = GetActorLocation();
	loc += GetActorForwardVector() * ((AxisValue * 100.f) / FrameRate);
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
	MoveTop(90.f);
}

void ASnakeHead::AngleBottom()
{
	MoveTop(-90.f);
}

void ASnakeHead::AngleRight()
{
	Turn(90.f);
}

void ASnakeHead::AngleLeft()
{
	Turn(-90.f);
}

void ASnakeHead::UpdateRotation() {
	if (YawValue != 0.f || PitchValue != 0.f || RollValue != 0.f) 
	{
		float deltaSec = GetWorld()->GetDeltaSeconds();
		if (deltaSec == 0.f)
		{
			return; //early return
		}
		float FrameRate = 1.f / deltaSec;
		if (FrameRate == 0.f)
		{
			return; //early return
		}
		//Update rotation
		FQuat QuatRotation = FQuat(FRotator(PitchValue, YawValue, RollValue));
		AddActorLocalRotation(QuatRotation * (100.f / FrameRate), false, 0, ETeleportType::None);
		//Spawn tag
		if (LastPiece) 
		{
			SpawnMovementTag();
		}
		YawValue = 0.f;
		RollValue = 0.f;
		PitchValue = 0.f;
	}
}

//Camera movement
void ASnakeHead::TurnRightCamera(float AxisValue) {
	if (AxisValue != 0.f) {
		float camYaw = SpringArm->GetRelativeRotation().Yaw;
		if (FMath::Abs(camYaw + AxisValue) < 90.f) //90: angle max
		{ 
			SpringArm->SetRelativeRotation(FRotator(SpringArm->GetRelativeRotation().Pitch, camYaw + AxisValue, 0.f));
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



/*Spawner*/
void ASnakeHead::SpawnPiece(FVector Location, FRotator Rotation)
{
	FActorSpawnParameters SpawnParams;
	ASnakePiece* SpawnedActorRef = GetWorld()->SpawnActor<ASnakePiece>(Piece, Location, Rotation, SpawnParams);
	if (SpawnedActorRef == NULL) 
	{
		return;
	}

	corps.Add(SpawnedActorRef);
	if (LastPiece) 
	{
		LastPiece->isLast = false;
	}
	LastPiece = SpawnedActorRef;	
}

void ASnakeHead::SpawnMovementTag() 
{
	FActorSpawnParameters SpawnParams;
	AMovementTag* SpawnedTagRef = GetWorld()->SpawnActor<AMovementTag>(MovementTag, GetActorLocation(), GetActorRotation(), SpawnParams);
}

//Others

void ASnakeHead::OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{
	if (OtherActor->ActorHasTag(FName("Snake.Piece"))) 
	{
		ASnakePiece* piece = dynamic_cast<ASnakePiece*>(OtherActor);
		if (piece && corps.IndexOfByKey(piece) != 0) //check if its not the first element of the tail (issue with collision)
		{
			Kill();
		}
	}
	else if (OtherActor->ActorHasTag(FName("Food"))) 
	{
		AFood* food = dynamic_cast<AFood*>(OtherActor);
		HitFood(food);
	}
}

void ASnakeHead::HitFood(AFood* food) 
{
	AddPiece();
	food->Destroy();
	if (Field) {
		Field->SpawnNextFood();
	}
	//Update HUD
	APlayerHUD* PlayerHUD = Cast<APlayerHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	if (PlayerHUD) 
	{
		Score += 1;
		PlayerHUD->UpdateScore(Score);
	}
}

void ASnakeHead::Kill() {
	
	for (ASnakePiece* piece : corps) {
		 piece->Destroy();
	}
	//Destroy();
	UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
}








