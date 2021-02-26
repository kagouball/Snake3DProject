// Fill out your copyright notice in the Description page of Project Settings.


#include "MovementTag.h"
#include "DrawDebugHelpers.h"
#include "SnakeHead.h"
#include "Math/BoxSphereBounds.h" 
#include "Components/SphereComponent.h" 

AMovementTag::AMovementTag()
{
	// Our root component will be a sphere that reacts to physics
	USphereComponent* SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("RootComponent"));
	RootComponent = SphereComponent;
	SphereComponent->InitSphereRadius(0.1f);
	SphereComponent->AttachTo(RootComponent);
	//listener
	OnActorBeginOverlap.AddDynamic(this, &AMovementTag::OnOverlapBegin);
	PitchValue, YawValue, RollValue = 0;
}

void AMovementTag::BeginPlay() {
	Super::BeginPlay();
	//DrawDebugSphere(GetWorld(), GetActorLocation(), GetSimpleCollisionRadius(), 3, FColor::Red, true, .5);
}

void AMovementTag::OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{
	if (OtherActor && (OtherActor != this) && OtherActor->ActorHasTag(FName("Snake.Piece"))) {
		//UE_LOG(LogTemp, Warning, TEXT("Aya, %s"), *OtherActor->GetName());
		//dosmthg
		ASnakePiece *piece = dynamic_cast<ASnakePiece*>(OtherActor);
		if (piece) {
			piece->AddMovementTag(this);
			//UE_LOG(LogTemp, Warning, TEXT("Trigger"));
			//piece->movementQueue.Add(this);
			/*piece->UpdateRotation(PitchValue, YawValue, RollValue);
			if (piece->isLast) {
				//UE_LOG(LogTemp, Warning, TEXT("Destroouuyy"), *OtherActor->GetName());
				Destroy();
			}*/
		}
	}
}
