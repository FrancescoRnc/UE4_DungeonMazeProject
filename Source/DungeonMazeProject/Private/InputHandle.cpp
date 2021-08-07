// Fill out your copyright notice in the Description page of Project Settings.


#include "InputHandle.h"

InputHandle::~InputHandle()
{
	ActionsData.Empty();
}

void InputHandle::RegisterNewAction(const FName& _actionName, const EInputEvent _eventType)
{
	FInputActionBinding newBinding = {_actionName, _eventType};
	refInputComponent->AddActionBinding(newBinding);

	auto indexOfLast = refInputComponent->GetNumActionBindings() - 1;
	AddData({indexOfLast, _actionName});
}

void InputHandle::SetDelegateToData(const int32 index, const FInputActionHandlerSignature& signature)
{
	ActionsData[index].Push(signature);	
}

void InputHandle::SetDelegateToAction(const int32 index)
{	
	refInputComponent->GetActionBinding(index).ActionDelegate = ActionsData[index].Peek();

	FInputActionUnifiedDelegate unified;
}

void InputHandle::AddData(const InputActionData& data)
{
	ActionsData.Add(data);
}
