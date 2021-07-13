// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */

// INPUT ACTION DATA
class DUNGEONMAZEPROJECT_API InputActionData
{
	public:
	InputActionData();
	InputActionData(int32 index, FName _name)
		: Index(index), Name(_name) {}
	InputActionData(int32 index, FName _name, FInputActionHandlerSignature _delegate)
		: Index(index), Name(_name), Delegate(_delegate) {}

	int32 Index = -1;
	FName Name = "None";	
	FInputActionHandlerSignature Delegate;
};

// INPUT ACTION SET
class DUNGEONMAZEPROJECT_API InputActionSet
{
	public:
	InputActionSet() : Actions({}) {}
	InputActionSet(TArray<InputActionData> _actions) : Actions(_actions) {}

	void Add(InputActionData newData) { Actions.Add(newData); }
	InputActionData& GetActionData(int32 index) { return Actions[index]; }
	void SetDelegateToAction(int32 index);

	private:
	TArray<InputActionData> Actions;
};


 //////////////////////////////
// INPUT HANDLE //////////////
class DUNGEONMAZEPROJECT_API InputHandle
{
	public:
	InputHandle() : ActionsData({}) { }
	InputHandle(UInputComponent* refComponent) : refInputComponent(refComponent), ActionsData({}) {}	
	~InputHandle();

	void RegisterNewAction(FName _actionName, EInputEvent _eventType);
	void SetDelegateToData(int32 index, FInputActionHandlerSignature signature);
	void SetDelegateToAction(int32 index);
	InputActionData& GetActionData(int32 index) { return ActionsData[index]; }
	void AddData(InputActionData data);

	private:
	UInputComponent* refInputComponent = 0;
	TArray<InputActionData> ActionsData;
};