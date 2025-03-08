#include "GeoFlowVectorMath.h"

GEOFLOWCORE_API FVector3f CompWiseAbs(FVector3f in) {
	return FVector3f(FMath::Abs(in.X), FMath::Abs(in.Y), FMath::Abs(in.Z));
}

GEOFLOWCORE_API FVector3f CompWiseMax(FVector3f a, FVector3f b) {
	return FVector3f(FMath::Max(a.X, b.X), FMath::Max(a.Y, b.Y), FMath::Max(a.Z, b.Z));
}

GEOFLOWCORE_API FVector3f CompWiseMin(FVector3f a, FVector3f b) {
	return FVector3f(FMath::Min(a.X, b.X), FMath::Min(a.Y, b.Y), FMath::Min(a.Z, b.Z));
}

GEOFLOWCORE_API FVector2f CompWiseAbs(FVector2f in) {
	return FVector2f(FMath::Abs(in.X), FMath::Abs(in.Y));
}

GEOFLOWCORE_API FVector2f CompWiseMax(FVector2f a, FVector2f b) {
	return FVector2f(FMath::Max(a.X, b.X), FMath::Max(a.Y, b.Y));
}

GEOFLOWCORE_API FVector2f CompWiseMin(FVector2f a, FVector2f b) {
	return FVector2f(FMath::Min(a.X, b.X), FMath::Min(a.Y, b.Y));
}
