#include "MarchingCubesComputeShader.h"
#include "GeoFlowRuntime/Public/MarchingCubesComputeShader.h"
#include "PixelShaderUtils.h"
//#include "RenderCore/Public/RenderGraphUtils.h"
#include "MeshPassProcessor.inl"
#include "StaticMeshResources.h"
#include "DynamicMeshBuilder.h"
#include "RenderGraphResources.h"
#include "GlobalShader.h"
#include "UnifiedBuffer.h"
#include "CanvasTypes.h"
#include "MaterialShader.h"

class GEOFLOWRUNTIME_API FGeoFlowMarchingCubesShader : public FGlobalShader {
public:
	DECLARE_GLOBAL_SHADER(FGeoFlowMarchingCubesShader);
	SHADER_USE_PARAMETER_STRUCT(FGeoFlowMarchingCubesShader, FGlobalShader);
	class FGeoFlowMarchingCubesShader_Perm_TEST : SHADER_PERMUTATION_INT("TEST", 1);
	using FPermutationDomain = TShaderPermutationDomain<
		FGeoFlowMarchingCubesShader_Perm_TEST
	>;

	BEGIN_SHADER_PARAMETER_STRUCT(FParameters, )

	SHADER_PARAMETER_RDG_BUFFER_SRV(StructuredBuffer<float3>, InputPoints)
	SHADER_PARAMETER_RDG_BUFFER_UAV(RWStructuredBuffer<float>, OutputVals)
	//SHADER_PARAMETER_RDG_BUFFER_UAV(AppendStructuredBuffer<FGeoflowComputeTriangle>, OutputTris)
	END_SHADER_PARAMETER_STRUCT()

public:
	static bool ShouldCompilePermutation(const FGlobalShaderPermutationParameters& Parameters)
	{
		const FPermutationDomain PermutationVector(Parameters.PermutationId);

		return true;
	}

	static void ModifyCompilationEnvironment(const FGlobalShaderPermutationParameters& Parameters, FShaderCompilerEnvironment& OutEnvironment)
	{
		FGlobalShader::ModifyCompilationEnvironment(Parameters, OutEnvironment);

		const FPermutationDomain PermutationVector(Parameters.PermutationId);

		/*
		* Here you define constants that can be used statically in the shader code.
		* Example:
		*/
		// OutEnvironment.SetDefine(TEXT("MY_CUSTOM_CONST"), TEXT("1"));

		/*
		* These defines are used in the thread count section of our shader
		*/
		OutEnvironment.SetDefine(TEXT("THREADS_X"), NUM_THREADS_MySimpleComputeShader_X);
		OutEnvironment.SetDefine(TEXT("THREADS_Y"), NUM_THREADS_MySimpleComputeShader_Y);
		OutEnvironment.SetDefine(TEXT("THREADS_Z"), NUM_THREADS_MySimpleComputeShader_Z);

		// This shader must support typed UAV load and we are testing if it is supported at runtime using RHIIsTypedUAVLoadSupported
		//OutEnvironment.CompilerFlags.Add(CFLAG_AllowTypedUAVLoads);

		// FForwardLightingParameters::ModifyCompilationEnvironment(Parameters.Platform, OutEnvironment);
	}
private:
};

// This will tell the engine to create the shader and where the shader entry point is.
//                            ShaderType                            ShaderPath                     Shader function name    Type
IMPLEMENT_GLOBAL_SHADER(FGeoFlowMarchingCubesShader, "/GeoFlowShaders/MarchingCubesCompute.usf", "GeoFlowMarchingCubes", SF_Compute);

void FGeoFlowMarchingCubesInterface::DispatchRenderThread(FRHICommandListImmediate& RHICmdList, FGeoFlowMarchingCubesComputeDispatchParams Params, TFunction<void(int numTris)> AsyncCallback) {
	FRDGBuilder GraphBuilder(RHICmdList);

	{
		//SCOPE_CYCLE_COUNTER(STAT_MySimpleComputeShader_Execute);
		DECLARE_GPU_STAT(GeoFlowMarchingCubes)
		RDG_EVENT_SCOPE(GraphBuilder, "GeoFlowMarchingCubes");
		RDG_GPU_STAT_SCOPE(GraphBuilder, GeoFlowMarchingCubes);

		typename FGeoFlowMarchingCubesShader::FPermutationDomain PermutationVector;

		// Add any static permutation options here
		// PermutationVector.Set<FMySimpleComputeShader::FMyPermutationName>(12345);

		TShaderMapRef<FGeoFlowMarchingCubesShader> ComputeShader(GetGlobalShaderMap(GMaxRHIFeatureLevel), PermutationVector);


		bool bIsShaderValid = ComputeShader.IsValid();

		if (bIsShaderValid) {
			FGeoFlowMarchingCubesShader::FParameters* PassParameters = GraphBuilder.AllocParameters<FGeoFlowMarchingCubesShader::FParameters>();


			const void* RawData = (void*)(*Params.inputPoints).GetData();
			uint32 NumInputs = (*Params.inputPoints).Num();
			uint32 InputSize = sizeof(float) * 3;
			FRDGBufferRef InputBuffer = CreateUploadBuffer(GraphBuilder, TEXT("InputBuffer"), InputSize, NumInputs, RawData, InputSize * NumInputs);

			PassParameters->InputPoints = GraphBuilder.CreateSRV(FRDGBufferSRVDesc(InputBuffer, PF_R32_SINT));
			
			FRDGBufferRef OutputBuffer = GraphBuilder.CreateBuffer(
				FRDGBufferDesc::CreateBufferDesc(sizeof(float), NumInputs ),
				TEXT("OutputBuffer"));
			PassParameters->OutputVals = GraphBuilder.CreateUAV(FRDGBufferUAVDesc(OutputBuffer, PF_R32_SINT));
			
			auto GroupCount = FComputeShaderUtils::GetGroupCount(FIntVector(Params.X, Params.Y, Params.Z), 1);
			GraphBuilder.AddPass(
				RDG_EVENT_NAME("ExecuteGeoFlowMarchingCubes"),
				PassParameters,
				ERDGPassFlags::AsyncCompute,
				[&PassParameters, ComputeShader, GroupCount](FRHIComputeCommandList& RHICmdList)
				{
					FComputeShaderUtils::Dispatch(RHICmdList, ComputeShader, *PassParameters, GroupCount);
				});

				FRHIGPUBufferReadback* GPUBufferReadback = new FRHIGPUBufferReadback(TEXT("ExecuteGeoFlowMarchingCubesOutput"));
				AddEnqueueCopyPass(GraphBuilder, GPUBufferReadback, OutputBuffer, 0u);
				auto RunnerFunc = [GPUBufferReadback, AsyncCallback, Params](auto&& RunnerFunc) -> void {
					if (GPUBufferReadback->IsReady()) {

						
						FGeoFlowMarchingCubesComputeDispatchParams* p = (FGeoFlowMarchingCubesComputeDispatchParams*)&Params;
						uint32 len = p->inputPoints->Num();
						float* Buffer = (float*)GPUBufferReadback->Lock(len * sizeof(float));
						p->outputVals->Reset();
						p->outputVals->Append(Buffer, len);

						GPUBufferReadback->Unlock();

						AsyncTask(ENamedThreads::GameThread, [AsyncCallback]() {AsyncCallback(0); });

						delete GPUBufferReadback;
					}
					else {
						AsyncTask(ENamedThreads::ActualRenderingThread, [RunnerFunc]() {
							RunnerFunc(RunnerFunc);
							});
					}
				};

				AsyncTask(ENamedThreads::ActualRenderingThread, [RunnerFunc]() {RunnerFunc(RunnerFunc);});

		}
		else {
			// We silently exit here as we don't want to crash the game if the shader is not found or has an error.

		}
	}

	GraphBuilder.Execute();
}
