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
		: Index(index), Name(_name) {DelegateStack.Push({});}
	InputActionData(int32 index, FName _name, FInputActionHandlerSignature _delegate)
		: Index(index), Name(_name), DelegateStack({_delegate}) {}

	void Push(FInputActionHandlerSignature newDelegate) { DelegateStack.Push(newDelegate); }
	FInputActionHandlerSignature& Peek() { return DelegateStack.Last(); }
	FInputActionHandlerSignature Pop() { auto pop = DelegateStack.Pop(false); return pop; }
	
	int32 Index = -1;
	FName Name = "None";	
	TArray<FInputActionHandlerSignature> DelegateStack;
};


 //////////////////////////////
/// INPUT HANDLE /////////////-
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
	FInputActionHandlerSignature& GetActionDelegate(int32 index) { return ActionsData[index].DelegateStack.Last(); }
	void AddData(InputActionData data);

	private:
	UInputComponent* refInputComponent = 0;
	TArray<InputActionData> ActionsData;
};
