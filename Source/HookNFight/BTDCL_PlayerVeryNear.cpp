// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDCL_PlayerVeryNear.h"

#include "C_Enemy.h"
#include "HookNFightCharacter.h"

#include "VisualLogger/VisualLogger.h"
#include "BehaviorTree/BTCompositeNode.h"

#include "BehaviorTree/BlackboardComponent.h"


bool UBTDCL_PlayerVeryNear::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	return (Cast<AC_Enemy>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("SelfActor"))->DetectUnique<AHookNFightCharacter>() != nullptr);
}

void UBTDCL_PlayerVeryNear::OnNodeDeactivation(FBehaviorTreeSearchData& SearchData, EBTNodeResult::Type NodeResult)
{
	if (NodeResult != EBTNodeResult::Aborted)
	{
		const UBlackboardComponent* BlackboardComp = SearchData.OwnerComp.GetBlackboardComponent();
		
		AC_Enemy* Enemy = Cast<AC_Enemy>(BlackboardComp->GetValueAsObject("SelfActor"));
		const bool bEvalResult = (Enemy->DetectUnique<AHookNFightCharacter>() != nullptr) && (!Enemy->Is_Hit());

		if (bEvalResult != IsInversed())
		{
			GetParentNode()->SetChildOverride(SearchData, GetChildIndex());
		}
	}
}
