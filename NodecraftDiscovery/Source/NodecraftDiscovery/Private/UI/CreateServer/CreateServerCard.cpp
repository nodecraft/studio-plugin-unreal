// Nodecraft, Inc. © 2012-2024, All Rights Reserved.


#include "UI/CreateServer/CreateServerCard.h"

#include "CommonBorder.h"
#include "Subsystems/MenuManagerSubsystem.h"
#include "UI/Foundation/NodecraftButtonBase.h"

void UCreateServerCard::NativeConstruct()
{
	Super::NativeConstruct();

	CreateServerButton->OnHovered().Clear();
	CreateServerButton->OnHovered().AddWeakLambda(this, [this]()
	{
		CardContentsContainer->SetContentColorAndOpacity(ContentTintOnHover);
	});

	CreateServerButton->OnUnhovered().Clear();
	CreateServerButton->OnUnhovered().AddWeakLambda(this, [this]()
	{
		CardContentsContainer->SetContentColorAndOpacity(ContentTintOnUnhover);
	});

	CreateServerButton->OnClicked().Clear();
	CreateServerButton->OnClicked().AddWeakLambda(this, [this]()
	{
		UMenuManagerSubsystem::Get().ShowCreateServerModal(CreateServerModalClass);
	});
}
