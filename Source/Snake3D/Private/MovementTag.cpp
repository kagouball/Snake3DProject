// Fill out your copyright notice in the Description page of Project Settings.


#include "MovementTag.h"
#include "DrawDebugHelpers.h"
#include "SnakeHead.h"
#include "Math/BoxSphereBounds.h" 
#include "Components/SphereComponent.h" 
#include "SnakePiece.h"

AMovementTag::AMovementTag()
{
	// Our root component will be a sphere that reacts to physics
	USphereComponent* SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("RootComponent"));
	RootComponent = SphereComponent;
	SphereComponent->InitSphereRadius(0.1f);
	//SphereComponent->SetupAttachment(RootComponent);
	RootComponent = SphereComponent;
	//listener
	OnActorBeginOverlap.AddDynamic(this, &AMovementTag::OnOverlapBegin);
	distanceToNext = -1;
	next = nullptr;
}

void AMovementTag::BeginPlay() {
	Super::BeginPlay();
	//Draw helper to see the trigger sphere
	//DrawDebugSphere(GetWorld(), GetActorLocation(), GetSimpleCollisionRadius(), 3, FColor::Red, true, .5);
}

void AMovementTag::OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{
	if (OtherActor && (OtherActor != this) && OtherActor->ActorHasTag(FName("Snake.Piece"))) {
		ASnakePiece *piece = dynamic_cast<ASnakePiece*>(OtherActor);
		if (piece) {
			//Warning : plz manage this directly in the setter
			if (piece->GetTarget() == nullptr) {
				piece->SetTarget(this,true);
			}
		}
	}
}

void AMovementTag::SetDistanceToNext(float distance) {
	distanceToNext = distance;
}

float AMovementTag::GetDistanceToNext()
{
	return distanceToNext;
}

void AMovementTag::SetNext(AMovementTag* nextTag)
{
	next = nextTag;
}

AMovementTag* AMovementTag::GetNext()
{
	return next;
}

void AMovementTag::Kill()
{
	next = nullptr;
	Destroy();
}
