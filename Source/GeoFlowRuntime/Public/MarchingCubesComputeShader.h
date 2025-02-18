#pragma once
#include "CoreMinimal.h"
#include "GenericPlatform/GenericPlatformMisc.h"
#include "Kismet/BlueprintAsyncActionBase.h"

#include "MarchingCubesComputeShader.generated.h"

//https://dev.epicgames.com/community/learning/tutorials/WkwJ/unreal-engine-simple-compute-shader-with-cpu-readback


struct GEOFLOWRUNTIME_API FGeoFlowMarchingCubesComputeDispatchParams {
	int X;
	int Y;
	int Z;
	//all points to sample, can be indexed using a stride instead of carrying a 2d array
	TArray<FVector3f>* inputPoints;
	TArray<float>* outputVals;
	FGeoFlowMarchingCubesComputeDispatchParams(int _x, int _y, int _z, TArray<FVector3f>& input, TArray<float>& output) : X(_x),Y(_y),Z(_z)
	{
		inputPoints = &(input);
		outputVals = &(output);
	}
};
class GEOFLOWRUNTIME_API FGeoFlowMarchingCubesInterface {
public:
	// Executes this shader on the render thread
	static void DispatchRenderThread(
		FRHICommandListImmediate& RHICmdList,
		FGeoFlowMarchingCubesComputeDispatchParams Params,
		TFunction<void(int errorCode)> AsyncCallback
	);

	// Executes this shader on the render thread from the game thread via EnqueueRenderThreadCommand
	static void DispatchGameThread(
		FGeoFlowMarchingCubesComputeDispatchParams Params,
		TFunction<void(int errorCode)> AsyncCallback
	)
	{
		ENQUEUE_RENDER_COMMAND(SceneDrawCompletion)(
			[Params, AsyncCallback](FRHICommandListImmediate& RHICmdList)
			{
				DispatchRenderThread(RHICmdList, Params, AsyncCallback);
			});
	}

	// Dispatches this shader. Can be called from any thread
	static void Dispatch(
		FGeoFlowMarchingCubesComputeDispatchParams Params,
		TFunction<void(int errorCode)> AsyncCallback
	)
	{
		if (IsInRenderingThread()) {
			DispatchRenderThread(GetImmediateCommandList_ForRenderCommand(), Params, AsyncCallback);
		}
		else {
			DispatchGameThread(Params, AsyncCallback);
		}
	}
};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGeoFlowMarchingCubes_AsyncExecutionCompleted, const int, errorCode);

UCLASS()
class GEOFLOWRUNTIME_API UGeoFlowMarchingCubes_AsyncExecution : public UBlueprintAsyncActionBase {
	GENERATED_BODY()
public:
	virtual void Activate() override {
		// Create a dispatch parameters struct and fill it the input array with our args
		FGeoFlowMarchingCubesComputeDispatchParams Params(1,1,1,*inputPoints,*outputVals);

		// Dispatch the compute shader and wait until it completes
		FGeoFlowMarchingCubesInterface::Dispatch(Params, [this](int errorCode) {
			this->Completed.Broadcast(errorCode);
			});
	}



	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", Category = "ComputeShader", WorldContext = "WorldContextObject"))
	static UGeoFlowMarchingCubes_AsyncExecution* ExecuteBaseComputeShader(UObject* WorldContextObject, TArray<FVector3f>& input, TArray<float>& output) {
		UGeoFlowMarchingCubes_AsyncExecution* Action = NewObject<UGeoFlowMarchingCubes_AsyncExecution>();
		Action->inputPoints = &input;
		Action->outputVals = &output;
		Action->RegisterWithGameInstance(WorldContextObject);

		return Action;
	}

	UPROPERTY(BlueprintAssignable)
	FOnGeoFlowMarchingCubes_AsyncExecutionCompleted Completed;

	TArray<FVector3f>* inputPoints = nullptr;
	TArray<float>* outputVals = nullptr;
};