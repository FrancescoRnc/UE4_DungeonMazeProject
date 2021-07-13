// Fill out your copyright notice in the Description page of Project Settings.


#include "InputHandle.h"

InputHandle::~InputHandle()
{
	ActionsData.Empty();
}

void InputHandle::RegisterNewAction(FName _actionName, EInputEvent _eventType)
{
	FInputActionBinding newBinding = {_actionName, _eventType};
	refInputComponent->AddActionBinding(newBinding);

	auto indexOfLast = refInputComponent->GetNumActionBindings() - 1;
	AddData({indexOfLast, _actionName});
}

void InputHandle::SetDelegateToData(int32 index, FInputActionHandlerSignature signature)
{
	ActionsData[index].Delegate = signature;	
}

void InputHandle::SetDelegateToAction(int32 index)
{	
	refInputComponent->GetActionBinding(index).ActionDelegate = ActionsData[index].Delegate;
}

void InputHandle::AddData(InputActionData data)
{
	ActionsData.Add(data);
}
