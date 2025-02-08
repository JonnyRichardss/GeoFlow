#pragma once
#include "CoreMinimal.h"
#include "GeoFlowComponent.h"

#include "GeoFlowActor.generated.h"

UCLASS()
class AGeoFlowActor : public AActor {
	GENERATED_BODY()
public:
	AGeoFlowActor();
private:
	UPROPERTY(Category = GeoFlow, VisibleAnywhere, BlueprintReadOnly, meta = (ExposeFunctionCategories = "Mesh,Rendering,Physics,Components", AllowPrivateAccess = "true"))
	TObjectPtr<UGeoFlowComponent> GeoFlow  = nullptr;

	virtual void OnConstruction(const FTransform& Transform) override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	// Called every frame
	virtual void Tick(float DeltaTime) override;

};