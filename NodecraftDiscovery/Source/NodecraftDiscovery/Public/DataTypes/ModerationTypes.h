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