#include "Models/Server/PlayerLeaveDataObjects.h"


FString FPlayerLeaveRequestData::ToString() const
{
	FString RequestString = "";
	RequestString += "SessionId: " + SessionId + "\n";
	RequestString += "Ip: " + Ip + "\n";
	if (AnalyticsSessionId.IsSet())
	{
		RequestString += "AnalyticsSessionId: " + AnalyticsSessionId.GetValue() + "\n";
	}
	if (SessionEndMessage.IsSet())
	{
		RequestString += "SessionEndMessage: " + SessionEndMessage.GetValue() + "\n";
	}
	return RequestString;
}

