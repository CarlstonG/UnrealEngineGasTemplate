// Copyright Whizz Entertianment


#include "Character/AuraCharacterBase.h"
#include "AbilitySystemComponent.h"

// Sets default values
AAuraCharacterBase::AAuraCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;



	//this is a socket component
	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");

	
	// Attached weapon tag to a socket
	Weapon->SetupAttachment(GetMesh(), FName("WeaponHandSocket"));

	//Enable to disable collision for object
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

//important this is from a module look at header
UAbilitySystemComponent* AAuraCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

// Called when the game starts or when spawned
void AAuraCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}


void AAuraCharacterBase::InitAbilityActorInfo()
{
}
//primary attribute
void AAuraCharacterBase::InitializePrimaryAttributes() const
{
	check(IsValid(GetAbilitySystemComponent()));
	check(DefaultPrimaryAttributes);
	const FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	const FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(DefaultPrimaryAttributes, 1.f, ContextHandle);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), GetAbilitySystemComponent());
}




