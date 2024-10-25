// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Application/IInputProcessor.h"


class FSlateApplication;
class FSlateUser;
struct FInputEvent;
struct FAnalogInputEvent;
struct FKeyEvent;
struct FPointerEvent;

enum class EAnalogStick : uint8
{
	Left,
	Right,
	Max,
};

class NODECRAFTDISCOVERY_API FNodecraftMenuNavInputProcessor : public IInputProcessor, public TSharedFromThis<FNodecraftMenuNavInputProcessor>
{
public:
	FNodecraftMenuNavInputProcessor();

	virtual ~FNodecraftMenuNavInputProcessor()
	{}

	static TSharedRef<FNodecraftMenuNavInputProcessor> CreateMenuNavInputProcessor()
    	{
    		TSharedRef<FNodecraftMenuNavInputProcessor> NewCursor = MakeShareable(new FNodecraftMenuNavInputProcessor());
    		return NewCursor;
    	}

	virtual void Tick(const float DeltaTime, FSlateApplication& SlateApp, TSharedRef<ICursor>) override;

	virtual bool HandleKeyDownEvent(FSlateApplication& SlateApp, const FKeyEvent& InKeyEvent) override;
	virtual bool HandleKeyUpEvent(FSlateApplication& SlateApp, const FKeyEvent& InKeyEvent) override;
	virtual bool HandleAnalogInputEvent(FSlateApplication& SlateApp, const FAnalogInputEvent& InAnalogInputEvent) override;
	virtual bool HandleMouseMoveEvent(FSlateApplication& SlateApp, const FPointerEvent& MouseEvent) override;
	virtual const TCHAR* GetDebugName() const override { return TEXT("AnalogCursor"); }


	virtual int32 GetOwnerUserIndex() const { return 0; };

	/** Getter */
	FORCEINLINE const FVector2D& GetAnalogValues( EAnalogStick Stick = EAnalogStick::Left ) const
	{
		return AnalogValues[ static_cast< uint8 >( Stick ) ];
	}

	/** Set the cached analog stick declinations to 0 */
	void ClearAnalogValues();

protected:

	virtual bool IsRelevantInput(const FInputEvent& InputEvent) const;
	virtual bool IsRelevantInput(const FKeyEvent& KeyEvent) const;
	virtual bool IsRelevantInput(const FAnalogInputEvent& AnalogInputEvent) const;
	virtual bool IsRelevantInput(const FPointerEvent& MouseEvent) const;

	float DeadZone;

private:

	FORCEINLINE FVector2D& GetAnalogValue( EAnalogStick Stick )
	{
		return AnalogValues[ static_cast< uint8 >( Stick ) ];
	}

	/** Input from the gamepad */
	FVector2D AnalogValues[ static_cast<uint8>( EAnalogStick::Max ) ];
};


