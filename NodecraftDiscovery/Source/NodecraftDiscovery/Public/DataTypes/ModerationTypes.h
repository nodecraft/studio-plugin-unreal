// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

UENUM()
enum class EModerationDuration : uint8
{
	Undefined,
	Temporary,
	Permanent,
	Expired
};

UENUM()
enum class EModerationScope : uint8
{
	Undefined,
	Server,
	Community,
	Game,
	Global
};

UENUM()
enum class EModerationAction : uint8
{
	Undefined,
	Kick,
	Ban,
	Unban,
	PromoteToModerator,
	DemoteModerator
};