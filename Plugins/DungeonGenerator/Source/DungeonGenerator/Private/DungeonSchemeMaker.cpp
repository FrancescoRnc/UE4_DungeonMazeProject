// Fill out your copyright notice in the Description page of Project Settings.


#include "DungeonSchemeMaker.h"


/*FDungeonSchemeMaker::FDungeonSchemeMaker()
{
	
}

FDungeonSchemeMaker::~FDungeonSchemeMaker()
{
	OutScheme.Empty();
	PrototypeScheme.Empty();
	CurrentScheme.Empty();
	SchemeStack.Empty();
}

void FDungeonSchemeMaker::MakeNewScheme()
{
	const int32 FlagsLength = SchemeLength / 4;
	const int32 RandomBeginIndex = FMath::RandRange(0, SchemeSize.X - 1);
	const int32 RandomEndIndex = (SchemeLength - 1) - FMath::RandRange(0, SchemeSize.X - 1);
	
	//TArray<FSchemeCell> Scheme = (PrototypeScheme);
	CurrentScheme = (PrototypeScheme);
	OutScheme.Init(0, SchemeLength);

	if (FlagsLength > 0)
	{	
		// Place flags (Unusable cells) to Random cells
		// (flags are a quarter the size of the Scheme)
		TArray<int32> ExcludedIndexes;
		ExcludedIndexes.Add(RandomBeginIndex);
		ExcludedIndexes.Add(RandomEndIndex);
		for (int32 Index = 0; Index < FlagsLength; Index++)
		{
			int32 RandInt = FMath::RandRange(0, SchemeLength - 1);
			while (ExcludedIndexes.Contains(RandInt))
			{
				RandInt = FMath::RandRange(0, SchemeLength - 1);
			}
			ExcludedIndexes.Add(RandInt);
			
			CurrentScheme[RandInt].CellStatus |= 8;
		}		
	}
	CurrentScheme[RandomBeginIndex].CellStatus |= 2;
	CurrentScheme[RandomEndIndex].CellStatus |= 4;
	
	bool DFS = DeepFirstSearch(CurrentScheme[RandomBeginIndex]);
	UE_LOG(LogTemp, Warning, TEXT("DFS Worked? %d"), DFS);
}

bool FDungeonSchemeMaker::DeepFirstSearch(FSchemeCell& InCell)
{
	SchemeStack.Push(InCell);
	InCell.CellStatus |= 1;
	bool Result = false;
	
	const TArray<FIntVector> Adjacents =
	{
		{ InCell.Coordinate.X, InCell.Coordinate.Y + 1, 0 },
		{ InCell.Coordinate.X - 1, InCell.Coordinate.Y, 0 },
		{ InCell.Coordinate.X, InCell.Coordinate.Y - 1, 0 },
		{ InCell.Coordinate.X + 1, InCell.Coordinate.Y, 0 }
	};
	
	for (int32 Index = 0; Index < Adjacents.Num(); Index++)
	{
		if (Adjacents[Index].X < 0 || Adjacents[Index].X >= SchemeSize.X ||
			Adjacents[Index].Y < 0 || Adjacents[Index].Y >= SchemeSize.Y)
		{
			continue; 
		}

		const int CurrentCellIndex = Adjacents[Index].X + (SchemeSize.X * Adjacents[Index].Y);
		FSchemeCell& SiblingCell = CurrentScheme[CurrentCellIndex];
		
		if (SiblingCell.CellStatus == 0)
		{
			const int32 OppositeDirectionIndex = FMath::Abs((Index + 2) % 4);
			InCell.CellType |= 1 << Index;
			SiblingCell.CellType |= 1 << OppositeDirectionIndex;

			OutScheme[InCell.Index] = InCell.CellType;
			Result = DeepFirstSearch(SiblingCell);
			//return DeepFirstSearch(SiblingCell);
		}
		else if (SiblingCell.CellStatus == 4)
		{
			//SchemeStack.Push(SiblingCell);
			const int32 OppositeDirectionIndex = FMath::Abs((Index + 2) % 4);
			InCell.CellType |= 1 << Index;
			SiblingCell.CellType |= 1 << OppositeDirectionIndex;

			OutScheme[InCell.Index] = InCell.CellType;
			OutScheme[SiblingCell.Index] = SiblingCell.CellType;
			//Result = true;
			//break;
			return true;
		}
	}
	SchemeStack.Pop();
	return Result;
}

const TArray<int32> FDungeonSchemeMaker::GetScheme() const
{
	TArray<int32> Scheme;
	Scheme.Init(0, SchemeLength);

	for (int32 Index = 0; Index < SchemeLength; Index++)
	{
		Scheme[Index] = CurrentScheme[Index].CellStatus;
	}
	
	return Scheme;
}*/


FGrid::FGrid()
{
	
}

FGrid::~FGrid()
{
	Cells.Empty();
	PathTrack.Empty();
	CellIndexLinks.Empty();
	Size = {0,0,0};
	Length = PathLength = 0;
}

// Grid
TArray<int32> FGrid::GetScheme()
{
	TArray<int32> Scheme;
	Scheme.Init(0, Length);
	for (int32 Index = 0; Index < Length; Index++)
	{
		Scheme[Index] = Cells[Index].CellPattern;
	}
	return Scheme;
}

// Second Implementation
FDungeonGridMaker::FDungeonGridMaker()
{
	
}

FDungeonGridMaker::~FDungeonGridMaker()
{
	PathTrack.Empty();
}

FGrid FDungeonGridMaker::GetGrid()
{
	return OutGrid;
}

FGridCell FDungeonGridMaker::ConstructCell(FGridCell& Cell, const int32 ProveniencePatternIndex) const
{
	const int32 ProveniencePattern = GetPatternFromIndex(ProveniencePatternIndex);
	Cell.CellPattern = ProveniencePattern;
	Cell.PathOrder = PathTrack.Num();

	int32 DirectionPatternIndex = FMath::RandRange(0, 3);
	FIntVector NewCoordinate = Cell.Coordinate + Directions[DirectionPatternIndex];
	while(	DirectionPatternIndex == ProveniencePatternIndex ||
			PathTrack.Contains(NewCoordinate))
	{
		DirectionPatternIndex = FMath::RandRange(0, 3);
		NewCoordinate = Cell.Coordinate + Directions[DirectionPatternIndex];
	}
	const int32 DirectionPattern = GetPatternFromIndex(DirectionPatternIndex);
	Cell.CellPattern |= DirectionPattern;
	
	return  Cell;
}

void FDungeonGridMaker::CheckMinMaxCoordinate(const FIntVector Coordinate)
{
	// MinPoint
	MinPoint.X = FMath::Min(MinPoint.X, Coordinate.X);
	MinPoint.Y = FMath::Min(MinPoint.Y, Coordinate.Y);

	// MaxPoint
	MaxPoint.X = FMath::Max(MaxPoint.X, Coordinate.X);
	MaxPoint.Y = FMath::Max(MaxPoint.Y, Coordinate.Y);
}

const int32 FDungeonGridMaker::GetOppositeIndex(const int32 Source) 
{
	return (Source + 2) % 4;
}

const int32 FDungeonGridMaker::GetPatternFromIndex(const int32 Index)
{
	return 1 << Index;
}

FGrid FDungeonGridMaker::MakeNewGrid(const FIntVector GridSize)
{
	FGrid NewGrid(GridSize);
	
	const FIntVector GridCenter = {GridRadius, GridRadius, 0};
	FIntVector NextCoordinate = GridCenter;
	int32 NextGridIndex = NextCoordinate.X + (NextCoordinate.Y * Size.X);
	int32 OppositePatternIndex;
	int32 NextPatternIndex;
	
	// First Cell setup
	StartPoint = GridCenter;
	PathTrack.Add(StartPoint);
	NewGrid[NextGridIndex].PathOrder = 1;
	CheckMinMaxCoordinate(GridCenter);

	// Preparing for next generated Cells
	OppositePatternIndex = 2;
	NextPatternIndex = 0;
	
	/*// Debug
	UE_LOG(LogTemp, Warning, TEXT("Opposite of 0 is %d"),
		OppositePatternIndex);
	// Debug
	UE_LOG(LogTemp, Warning, TEXT("Index chosen: %d Adding (%d, %d) to Coords"),
		NextPatternIndex, Directions[NextPatternIndex].X, Directions[NextPatternIndex].Y);
	UE_LOG(LogTemp, Warning, TEXT("Cell N'1 created at (%d, %d) -> Going %d (Next Cell at (%d, %d))"),
		StartPoint.X, StartPoint.Y, NextPatternIndex,
		StartPoint.X + Directions[NextPatternIndex].X,
		StartPoint.Y + Directions[NextPatternIndex].Y);
	UE_LOG(LogTemp, Warning, TEXT("+--------------------------------+"));*/
	
	NextCoordinate = StartPoint + Directions[NextPatternIndex];
	CheckMinMaxCoordinate(NextCoordinate);
	NextGridIndex = NextCoordinate.X + (NextCoordinate.Y * Size.X);

	for (int32 Index = 0; Index < RoomsCount; Index++)
	{
		// Current Cell setup
		PathTrack.Add(NextCoordinate);
		NewGrid[NextGridIndex].PathOrder = PathTrack.Num();		

		// Preparing data for the next Cell
		OppositePatternIndex = GetOppositeIndex(NextPatternIndex);
		
		/*// Debug
		UE_LOG(LogTemp, Warning, TEXT("Opposite of %d is %d"),
			NextPatternIndex, OppositePatternIndex);*/

		// Random direction excluding opposite direction and already existing Cell
		int32 NewPatternIndex = FMath::RandRange(0, 3);
		FIntVector NewCoordinate = NextCoordinate + Directions[NewPatternIndex];
		while(	NewPatternIndex == OppositePatternIndex ||
				PathTrack.Contains(NewCoordinate))
		{
			/*// Debug
			if (NewPatternIndex == OppositePatternIndex)
         	{
         		UE_LOG(LogTemp, Warning, TEXT("! Cannot use Index %d because provenience Index is %d"),
         		NewPatternIndex, OppositePatternIndex);
         	}
			if (PathTrack.Contains(NewCoordinate))
			{
				UE_LOG(LogTemp, Warning, TEXT("! Cell at (%d, %d) is Overlapping!"),
				NewCoordinate.X, NewCoordinate.Y);
			}*/
			
			NewPatternIndex = FMath::RandRange(0, 3);
			NewCoordinate = NextCoordinate + Directions[NewPatternIndex];
		}

		/*// Debug
		UE_LOG(LogTemp, Warning, TEXT("Now going %d and Adding (%d, %d) to Position"),
			NewPatternIndex, Directions[NewPatternIndex].X, Directions[NewPatternIndex].Y);
		UE_LOG(LogTemp, Warning, TEXT("Cell N'%d created at (%d, %d) -> Going %d (Provenience is %d | Next Cell at (%d, %d))"),
			InGrid[NextGridIndex].PathOrder, NextCoordinate.X, NextCoordinate.Y,
			NewPatternIndex, OppositePatternIndex,
			NewCoordinate.X, NewCoordinate.Y);
		UE_LOG(LogTemp, Warning, TEXT("+--------------------------------+"));*/
		
		NextPatternIndex = NewPatternIndex;
		
		NextCoordinate = NewCoordinate;
		CheckMinMaxCoordinate(NextCoordinate);
		NextGridIndex = NextCoordinate.X + (NextCoordinate.Y * Size.X);
		
	}	
	
	// Last Cell setup
	PathTrack.Add(NextCoordinate);
	NewGrid[NextGridIndex].PathOrder = PathTrack.Num();	
	EndPoint = NextCoordinate;

	/*// Debug
	UE_LOG(LogTemp, Warning, TEXT("Last Cell N'%d created at (%d, %d) -> Provenience is %d"),
		InGrid[NextGridIndex].PathOrder,
		NextCoordinate.X, NextCoordinate.Y,
		OppositePatternIndex);	
	UE_LOG(LogTemp, Warning, TEXT("+--------------------------------+"));*/

	NewGrid.PathTrack = PathTrack;
	NewGrid.PathLength = PathTrack.Num();
	return NewGrid;
}

void FDungeonGridMaker::LinkGridCells(FGrid& Grid)
{
	TArray<FGridCell> Cells;
	Cells.Init({}, PathTrack.Num());
	for (int32 Index = 0; Index < PathTrack.Num(); Index++)
	{
		const FIntVector Coord = Grid.PathTrack[Index];
		Cells[Index] = Grid.GetCell(Coord.X, Coord.Y);
	}

	Cells[0].CellPattern = 1;
	Grid.GetCell(Cells[0].Coordinate.X, Cells[0].Coordinate.Y).CellPattern = 1;
	Grid.CellIndexLinks.Add(TTuple<int32, int32>(0, 1));
	for (int32 Index = 1; Index < PathTrack.Num(); Index++)
	{
		int32 NextDirectionPattern{0}, SourceDirection{0}, SourceIndex{0};        
		for (int32 Direction = 0; Direction < Directions.Num(); Direction++)
		{
			if (Index < PathTrack.Num() - 1)
			{
				const FIntVector CheckForward = PathTrack[Index + 1] - PathTrack[Index];
                if (CheckForward.X == Directions[Direction].X &&
                	CheckForward.Y == Directions[Direction].Y)
                {
                	NextDirectionPattern = GetPatternFromIndex(Direction);
                }
			}
			
			const FIntVector CheckBackward = PathTrack[Index - 1] - PathTrack[Index];
			if (CheckBackward.X == Directions[Direction].X &&
				CheckBackward.Y == Directions[Direction].Y)
			{
				SourceDirection = GetPatternFromIndex(Direction);
				SourceIndex = Direction;
			}
		}
		Cells[Index].CellPattern = NextDirectionPattern | SourceDirection;
		const int32 I = Cells[Index].Coordinate.X + (Cells[Index].Coordinate.Y * Grid.Size.X);
		Grid[I].CellPattern = NextDirectionPattern | SourceDirection;

		// Linking
		const int32 OppositeSourceDirection =
			GetPatternFromIndex(GetOppositeIndex(SourceIndex));
		Grid.CellIndexLinks.Add(TTuple<int32, int32>
			(OppositeSourceDirection, SourceDirection));
		Grid.CellIndexLinks.Add(TTuple<int32, int32>
			(SourceDirection, OppositeSourceDirection));

		// TROVA UN MODO DI LINKARE LE DOORS
	}	
}

FGrid FDungeonGridMaker::CropGrid(const FGrid& SourceGrid)
{	
	const FIntVector NewSize =
	{
		(MaxPoint.X - MinPoint.X) + 1,
		(MaxPoint.Y - MinPoint.Y) + 1,
		0
	};
	const int32 NewLength = NewSize.X * NewSize.Y;
	
	FGrid Crop(NewSize);
	Crop.PathTrack.Init({0,0,0}, SourceGrid.PathLength);
	Crop.PathLength = SourceGrid.PathLength;
	
	for(int32 Index = 0; Index < PathTrack.Num(); Index++)
	{
		const FIntVector CropCoords = PathTrack[Index] - MinPoint;
		const int32 CropIndex = CropCoords.X + (CropCoords.Y * NewSize.X);
		const int32 GridIndex = PathTrack[Index].X +
								(PathTrack[Index].Y * Size.X);
		Crop[CropIndex].PathOrder = SourceGrid.Cells[GridIndex].PathOrder;
		
		Crop.PathTrack[Index] = PathTrack[Index] - MinPoint;
	}
	
	return Crop;
}

void FDungeonGridMaker::DebugGrid(const FGrid FullGrid)
{
	UE_LOG(LogTemp, Warning, TEXT("Path Length: %d"), PathTrack.Num());
	UE_LOG(LogTemp, Warning, TEXT("Path Order"));
	for (int32 Y = FullGrid.Size.Y - 1; Y >= 0; Y--)
	{
		FString Row("");
		for (int32 X = FullGrid.Size.X - 1; X >= 0; X--)
		{
			const int32 Index = X + (Y * FullGrid.Size.X);
			Row.Append(FullGrid.Cells[Index].PathOrder == 0 ?
				TEXT("[  ]") : FString::Printf(TEXT("[%02d]"), FullGrid.Cells[Index].PathOrder));
		}
		UE_LOG(LogTemp, Warning, TEXT("%s"), *Row);
	}
	UE_LOG(LogTemp, Warning, TEXT("+--------------------------------+"));
	UE_LOG(LogTemp, Warning, TEXT("Path Pattern"));
	for (int32 Y = FullGrid.Size.Y - 1; Y >= 0; Y--)
	{
		FString Row("");
		for (int32 X = FullGrid.Size.X - 1; X >= 0; X--)
		{
			const int32 Index = X + (Y * FullGrid.Size.X);
			Row.Append(FullGrid.Cells[Index].PathOrder == 0 ?
				TEXT("[  ]") : FString::Printf(TEXT("[%02d]"), FullGrid.Cells[Index].CellPattern));
		}
		UE_LOG(LogTemp, Warning, TEXT("%s"), *Row);
	}
}
