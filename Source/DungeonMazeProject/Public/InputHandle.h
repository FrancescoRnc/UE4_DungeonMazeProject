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

	void RegisterNewAction(const FName& _actionName, const EInputEvent _eventType);
	void SetDelegateToData(const int32 index, const FInputActionHandlerSignature& signature);
	void SetDelegateToAction(const int32 index);
	InputActionData& GetActionData(const int32 index) { return ActionsData[index]; }
	FInputActionHandlerSignature& GetActionDelegate(const int32 index) { return ActionsData[index].DelegateStack.Last(); }
	void AddData(const InputActionData& data);

	private:
	UInputComponent* refInputComponent = 0;
	TArray<InputActionData> ActionsData;
};
