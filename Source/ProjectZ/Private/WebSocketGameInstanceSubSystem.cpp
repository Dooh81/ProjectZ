// Fill out your copyright notice in the Description page of Project Settings.


#include "WebSocketGameInstanceSubSystem.h"

#include "RestApiGameInstanceSubSystem.h"
#include "TextReaderComponent.h"
#include "WebSocketsModule.h"
#include "ZGameInstance.h"
#include "Commons/Commons.h"
#include "Kismet/GameplayStatics.h"

UWebSocketGameInstanceSubSystem::UWebSocketGameInstanceSubSystem()
{
	UTextReaderComponent* TextReader = CreateDefaultSubobject<UTextReaderComponent>(TEXT("TextReaderComp"));
	WebSocketUrl = TextReader->ReadFile("Urls/WebsocketURL.txt");
}

void UWebSocketGameInstanceSubSystem::SetupWS()
{
	//소켓 접속 로직

	if (!FModuleManager::Get().IsModuleLoaded("WebSockets"))
	{
		FModuleManager::Get().LoadModule("WebSockets");
	}

	WebSocket = FWebSocketsModule::Get().CreateWebSocket(WebSocketUrl);

	WebSocket->OnConnected().AddLambda([]()
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, "Successfully connected");
	});

	WebSocket->OnConnectionError().AddLambda([](const FString& Error)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "Error :" + Error);
	});

	WebSocket->OnClosed().AddLambda([](int32 StatusCode, const FString& Reason, bool bWasClean)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, bWasClean ? FColor::Green : FColor::Red,
		                                 "Connection closed:" + Reason);
	});


	WebSocket->OnMessageSent().AddLambda([](const FString& MessageString)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, "Sent Message: " + MessageString);
	});

	//Message state
	//WebSocket->OnMessage().AddLambda([](const FString& MessageString)
	WebSocket->OnMessage().AddUObject(this, &UWebSocketGameInstanceSubSystem::OnReceived);


	WebSocket->Connect();
}

void UWebSocketGameInstanceSubSystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	//초기화시 웹소켓 세팅
	SetupWS();
	OnReceived(RegisteredJSON);
	// TArray<int32> Nums;
	// Nums.Add(1111111);
	// Nums.Add(2222222);
	// Nums.Add(3333333);
	// Nums.Add(4444444);
	// Nums.Add(5555555);
	// Nums.Add(6666666);
	// Nums.Add(7777777);
	// Nums.Add(8888888);
	//
	// OnSentMessage(E_WS_Command::ChangePlayerEquipment, Nums);

	//OnSentMessage(E_WS_Command::Greet, "");
	//OnSentMessage(E_WS_Command::ChangePlayerEquipment, Nums);

	//OnSentMessage(E_WS_Command::ChangePlayerEquipment, "", Nums );
	//OnReceived(UpdatePlayerEquipmentJSON);

	//OnSentMessage(E_WS_Command::AddExp, "");
}

void UWebSocketGameInstanceSubSystem::Deinitialize()
{
	Super::Deinitialize();

	//초기화 해제시 웹소켓 접속을 끊고 딜리게이트 이벤트를 해제한다.
	if (WebSocket->IsConnected())
	{
		WebSocket->Close();
	}

	Dele_LoginCompleteUsernameSetting_OneParam.Unbind();
	Dele_ChatMsg.Unbind();
}

void UWebSocketGameInstanceSubSystem::OnReceived(const FString& MessageString)
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, "WS Received Message: " + MessageString);
	UE_LOG(LogTemp, Warning, TEXT("Received Message: %s"), *MessageString);

	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(MessageString);
	if (FJsonSerializer::Deserialize(Reader, JsonObject))
	{
		if (JsonObject->HasField("event"))
		{
			FString State = JsonObject->GetStringField("event");
			//FString Data = JsonObject->GetStringField("data");

			E_WS_SSE SSE_Key = GetSSEEnum(State);

			switch (SSE_Key)
			{
			case E_WS_SSE::None:

				break;
			case E_WS_SSE::LoginAccepted:

				break;
			case E_WS_SSE::Registered:
				{
					FPlayerAvatarInfo PlayerAvatarInfo;
					TSharedPtr<FJsonObject> Object = JsonObject->GetObjectField("data");
					FString UserName = Object->GetStringField("UserName");
					int32 Country = Object->GetIntegerField("Country");
					int32 Level = Object->GetIntegerField("Level");
					int32 Exp = Object->GetIntegerField("Exp");
					int32 Point = Object->GetIntegerField("Point");
					int32 Gender = Object->GetIntegerField("Gender");
					int32 BodyColor = Object->GetIntegerField("BodyColor");
					int32 Bicycle = Object->GetIntegerField("Bicycle");

					TSharedPtr<FJsonObject> PlayerEquipmentInfoObject = Object->GetObjectField(
						"PlayerEquipmentInfo");

					int32 FrameIndex = PlayerEquipmentInfoObject->GetIntegerField("FrameIndex");
					int32 WheelIndex = PlayerEquipmentInfoObject->GetIntegerField("WheelIndex");
					int32 HeadgearIndex = PlayerEquipmentInfoObject->GetIntegerField("HeadgearIndex");
					int32 GlassesIndex = PlayerEquipmentInfoObject->GetIntegerField("GlassesIndex");
					int32 JerseyIndex = PlayerEquipmentInfoObject->GetIntegerField("JerseyIndex");
					int32 GlovesIndex = PlayerEquipmentInfoObject->GetIntegerField("GlovesIndex");
					int32 SocksIndex = PlayerEquipmentInfoObject->GetIntegerField("SocksIndex");
					int32 ShoesIndex = PlayerEquipmentInfoObject->GetIntegerField("ShoesIndex");

					FPlayerEquipmentInfo PlayerEquipmentInfo;
					PlayerEquipmentInfo.FrameIndex = FrameIndex;
					PlayerEquipmentInfo.WheelIndex = WheelIndex;
					PlayerEquipmentInfo.HeadgearIndex = HeadgearIndex;
					PlayerEquipmentInfo.GlassesIndex = GlassesIndex;
					PlayerEquipmentInfo.JerseyIndex = JerseyIndex;
					PlayerEquipmentInfo.GlovesIndex = GlovesIndex;
					PlayerEquipmentInfo.SocksIndex = SocksIndex;
					PlayerEquipmentInfo.ShoesIndex = ShoesIndex;

					TArray<TSharedPtr<FJsonValue>> Items = Object->GetArrayField("Items");
					TArray<int32> ItemsArray;
					for (auto& Item : Items)
					{
						if (Item->Type == EJson::Number)
						{
							int32 NumberValue = Item->AsNumber();
							ItemsArray.Add(NumberValue);
						}
						else
						{
							UE_LOG(LogTemp, Warning, TEXT("Num is not a number"));
						}
					}

					PlayerAvatarInfo.UserName = UserName;
					PlayerAvatarInfo.Country = ConvertIntToEnum<ECountry>(Country);
					PlayerAvatarInfo.Level = Level;
					PlayerAvatarInfo.Exp = Exp;
					PlayerAvatarInfo.Point = Point;
					PlayerAvatarInfo.Gender = ConvertIntToEnum<EGenderType>(Gender);
					PlayerAvatarInfo.BodyColor = ConvertIntToEnum<EBodyColor>(BodyColor);
					PlayerAvatarInfo.Bicycle = ConvertIntToEnum<EBicycleType>(Bicycle);
					PlayerAvatarInfo.PlayerEquipmentInfo = PlayerEquipmentInfo;
					PlayerAvatarInfo.Items = ItemsArray;

					UE_LOG(LogTemp, Warning, TEXT("U: %s, C: %d, L: %d, ItemsCount: %d"), *PlayerAvatarInfo.UserName,
					       PlayerAvatarInfo.Country, PlayerAvatarInfo.Level, PlayerAvatarInfo.Items.Num());

					UZGameInstance* GameInstance = Cast<UZGameInstance>(GetGameInstance());
					if (GameInstance)
					{
						GameInstance->SetPlayerAvatarInfo(PlayerAvatarInfo);
					}
				}
				break;
			case E_WS_SSE::UpdatePlayerEquipment:
				{
					//UE_LOG(LogTemp, Warning, TEXT("UpdatePlayerEquipmentJSON : %s"), *MessageString);
					TSharedPtr<FJsonObject> Object = JsonObject->GetObjectField("data");

					int32 FrameIndex = Object->GetNumberField("FrameIndex");
					int32 WheelIndex = Object->GetNumberField("WheelIndex");
					int32 HeadgearIndex = Object->GetNumberField("HeadgearIndex");
					int32 GlassesIndex = Object->GetNumberField("GlassesIndex");
					int32 JerseyIndex = Object->GetNumberField("JerseyIndex");
					int32 GlovesIndex = Object->GetNumberField("GlovesIndex");
					int32 SocksIndex = Object->GetNumberField("SocksIndex");
					int32 ShoesIndex = Object->GetNumberField("ShoesIndex");

					//UE_LOG(LogTemp,Warning,TEXT("FrameIndex: %d, ShoesIndex: %d"), FrameIndex, ShoesIndex);

					FPlayerEquipmentInfo PlayerEquipmentInfo;
					PlayerEquipmentInfo.FrameIndex = FrameIndex;
					PlayerEquipmentInfo.WheelIndex = WheelIndex;
					PlayerEquipmentInfo.HeadgearIndex = HeadgearIndex;
					PlayerEquipmentInfo.GlassesIndex = GlassesIndex;
					PlayerEquipmentInfo.JerseyIndex = JerseyIndex;
					PlayerEquipmentInfo.GlovesIndex = GlovesIndex;
					PlayerEquipmentInfo.SocksIndex = SocksIndex;
					PlayerEquipmentInfo.ShoesIndex = ShoesIndex;

					UZGameInstance* GameInstance = Cast<UZGameInstance>(GetGameInstance());
					if (GameInstance)
					{
						FPlayerAvatarInfo PlayerAvatarInfo = GameInstance->GetAvatarInfo();
						PlayerAvatarInfo.PlayerEquipmentInfo = PlayerEquipmentInfo;

						GameInstance->SetPlayerAvatarInfo(PlayerAvatarInfo);
					}
				}
				break;
			case E_WS_SSE::UpdateExp:
				{
					TSharedPtr<FJsonObject> Object = JsonObject->GetObjectField("data");
					int32 Level = Object->GetNumberField("Level");
					int32 Exp = Object->GetNumberField("Exp");

					UZGameInstance* GameInstance = Cast<UZGameInstance>(GetGameInstance());
					if (GameInstance)
					{
						FPlayerAvatarInfo PlayerAvatarInfo = GameInstance->GetAvatarInfo();
						PlayerAvatarInfo.Level = Level;
						PlayerAvatarInfo.Exp = Exp;

						GameInstance->SetPlayerAvatarInfo(PlayerAvatarInfo);
					}
				}
				break;
			case E_WS_SSE::UpdatePoint:
				{
					TSharedPtr<FJsonObject> Object = JsonObject->GetObjectField("data");
					int32 Point = Object->GetNumberField("Point");

					UZGameInstance* GameInstance = Cast<UZGameInstance>(GetGameInstance());
					if (GameInstance)
					{
						FPlayerAvatarInfo PlayerAvatarInfo = GameInstance->GetAvatarInfo();
						PlayerAvatarInfo.Point = Point;

						GameInstance->SetPlayerAvatarInfo(PlayerAvatarInfo);
					}
				}
				break;
			case E_WS_SSE::UpdateGender:
				{
					TSharedPtr<FJsonObject> Object = JsonObject->GetObjectField("data");
					int32 Gender = Object->GetNumberField("Gender");

					UZGameInstance* GameInstance = Cast<UZGameInstance>(GetGameInstance());
					if (GameInstance)
					{
						FPlayerAvatarInfo PlayerAvatarInfo = GameInstance->GetAvatarInfo();
						PlayerAvatarInfo.Gender = ConvertIntToEnum<EGenderType>(Gender);

						GameInstance->SetPlayerAvatarInfo(PlayerAvatarInfo);
					}
				}
				break;
			case E_WS_SSE::UpdateBodyColor:
				{
					TSharedPtr<FJsonObject> Object = JsonObject->GetObjectField("data");
					int32 BodyColor = Object->GetNumberField("BodyColor");

					UZGameInstance* GameInstance = Cast<UZGameInstance>(GetGameInstance());
					if (GameInstance)
					{
						FPlayerAvatarInfo PlayerAvatarInfo = GameInstance->GetAvatarInfo();
						PlayerAvatarInfo.BodyColor = ConvertIntToEnum<EBodyColor>(BodyColor);

						GameInstance->SetPlayerAvatarInfo(PlayerAvatarInfo);
					}
				}
				break;
			case E_WS_SSE::UpdateBicycle:
				{
					TSharedPtr<FJsonObject> Object = JsonObject->GetObjectField("data");
					int32 Bicycle = Object->GetNumberField("Bicycle");

					UZGameInstance* GameInstance = Cast<UZGameInstance>(GetGameInstance());
					if (GameInstance)
					{
						FPlayerAvatarInfo PlayerAvatarInfo = GameInstance->GetAvatarInfo();
						PlayerAvatarInfo.Bicycle = ConvertIntToEnum<EBicycleType>(Bicycle);

						GameInstance->SetPlayerAvatarInfo(PlayerAvatarInfo);
					}
				}
				break;
			case E_WS_SSE::UpdateItem:
				{
					TSharedPtr<FJsonObject> Object = JsonObject->GetObjectField("data");
					TArray<TSharedPtr<FJsonValue>> Items = Object->GetArrayField("Items");
					TArray<int32> ItemsArray;
					for (auto& Item : Items)
					{
						if (Item->Type == EJson::Number)
						{
							int32 NumberValue = Item->AsNumber();
							ItemsArray.Add(NumberValue);
						}
						else
						{
							UE_LOG(LogTemp, Warning, TEXT("Num is not a number"));
						}
					}

					UZGameInstance* GameInstance = Cast<UZGameInstance>(GetGameInstance());
					if (GameInstance)
					{
						FPlayerAvatarInfo PlayerAvatarInfo = GameInstance->GetAvatarInfo();
						PlayerAvatarInfo.Items = ItemsArray;

						GameInstance->SetPlayerAvatarInfo(PlayerAvatarInfo);
					}
				}
				break;
			case E_WS_SSE::UpdateContry:
				{
					TSharedPtr<FJsonObject> Object = JsonObject->GetObjectField("data");
					int32 Contry = Object->GetNumberField("Contry");

					UZGameInstance* GameInstance = Cast<UZGameInstance>(GetGameInstance());
					if (GameInstance)
					{
						FPlayerAvatarInfo PlayerAvatarInfo = GameInstance->GetAvatarInfo();
						PlayerAvatarInfo.Country = ConvertIntToEnum<ECountry>(Contry);

						GameInstance->SetPlayerAvatarInfo(PlayerAvatarInfo);
					}
				}
				break;
			case E_WS_SSE::ReturnPlayerInfo:
				{
					FPlayerAvatarInfo PlayerAvatarInfo;
					TSharedPtr<FJsonObject> Object = JsonObject->GetObjectField("data");
					FString UserName = Object->GetStringField("UserName");
					int32 Country = Object->GetIntegerField("Country");
					int32 Level = Object->GetIntegerField("Level");
					int32 Exp = Object->GetIntegerField("Exp");
					int32 Point = Object->GetIntegerField("Point");
					int32 Gender = Object->GetIntegerField("Gender");
					int32 BodyColor = Object->GetIntegerField("BodyColor");
					int32 Bicycle = Object->GetIntegerField("Bicycle");

					TSharedPtr<FJsonObject> PlayerEquipmentInfoObject = Object->GetObjectField(
						"PlayerEquipmentInfo");

					int32 FrameIndex = PlayerEquipmentInfoObject->GetIntegerField("FrameIndex");
					int32 WheelIndex = PlayerEquipmentInfoObject->GetIntegerField("WheelIndex");
					int32 HeadgearIndex = PlayerEquipmentInfoObject->GetIntegerField("HeadgearIndex");
					int32 GlassesIndex = PlayerEquipmentInfoObject->GetIntegerField("GlassesIndex");
					int32 JerseyIndex = PlayerEquipmentInfoObject->GetIntegerField("JerseyIndex");
					int32 GlovesIndex = PlayerEquipmentInfoObject->GetIntegerField("GlovesIndex");
					int32 SocksIndex = PlayerEquipmentInfoObject->GetIntegerField("SocksIndex");
					int32 ShoesIndex = PlayerEquipmentInfoObject->GetIntegerField("ShoesIndex");

					FPlayerEquipmentInfo PlayerEquipmentInfo;
					PlayerEquipmentInfo.FrameIndex = FrameIndex;
					PlayerEquipmentInfo.WheelIndex = WheelIndex;
					PlayerEquipmentInfo.HeadgearIndex = HeadgearIndex;
					PlayerEquipmentInfo.GlassesIndex = GlassesIndex;
					PlayerEquipmentInfo.JerseyIndex = JerseyIndex;
					PlayerEquipmentInfo.GlovesIndex = GlovesIndex;
					PlayerEquipmentInfo.SocksIndex = SocksIndex;
					PlayerEquipmentInfo.ShoesIndex = ShoesIndex;

					TArray<TSharedPtr<FJsonValue>> Items = Object->GetArrayField("Items");
					TArray<int32> ItemsArray;
					for (auto& Item : Items)
					{
						if (Item->Type == EJson::Number)
						{
							int32 NumberValue = Item->AsNumber();
							ItemsArray.Add(NumberValue);
						}
						else
						{
							UE_LOG(LogTemp, Warning, TEXT("Num is not a number"));
						}
					}

					PlayerAvatarInfo.UserName = UserName;
					PlayerAvatarInfo.Country = ConvertIntToEnum<ECountry>(Country);
					PlayerAvatarInfo.Level = Level;
					PlayerAvatarInfo.Exp = Exp;
					PlayerAvatarInfo.Point = Point;
					PlayerAvatarInfo.Gender = ConvertIntToEnum<EGenderType>(Gender);
					PlayerAvatarInfo.BodyColor = ConvertIntToEnum<EBodyColor>(BodyColor);
					PlayerAvatarInfo.Bicycle = ConvertIntToEnum<EBicycleType>(Bicycle);
					PlayerAvatarInfo.PlayerEquipmentInfo = PlayerEquipmentInfo;
					PlayerAvatarInfo.Items = ItemsArray;

					UE_LOG(LogTemp, Warning, TEXT("U: %s, C: %d, L: %d, ItemsCount: %d"), *PlayerAvatarInfo.UserName,
					       PlayerAvatarInfo.Country, PlayerAvatarInfo.Level, PlayerAvatarInfo.Items.Num());

					UZGameInstance* GameInstance = Cast<UZGameInstance>(GetGameInstance());
					if (GameInstance)
					{
						GameInstance->SetPlayerAvatarInfo(PlayerAvatarInfo);
					}
				}
				break;
			// AHUD* Hud = UGameplayStatics::GetPlayerController(this, 0)->GetHUD();
			// AMainMenuHUD* MainMenuHUD = Cast<AMainMenuHUD>(Hud);
			// if (MainMenuHUD != nullptr)
			// {
			// 	switch (SSE_Key)
			// 	{
			// 	case E_WS_SSE::None:
			//
			// 		break;
			// 	case E_WS_SSE::LoginAccepted:
			//
			// 		break;
			// 	case E_WS_SSE::Registered:
			// 		{
			// 			Username = JsonObject->GetObjectField("data")->GetStringField(
			// 						"username");
			// 			UserCountry = JsonObject->GetObjectField("data")->GetStringField(
			// 				"userCountry");
			//
			// 			UE_LOG(LogTemp, Warning, TEXT("contry: %s"), *UserCountry);
			// 			if (Dele_LoginCompleteUsernameSetting_OneParam.IsBound() == true)
			// 				Dele_LoginCompleteUsernameSetting_OneParam.Execute(
			// 					Username);
			// 		}
			//
			// 		break;
			// 	case E_WS_SSE::Added:
			// 		if (MainMenuHUD != nullptr)
			// 		{
			// 			MainMenuHUD->ShowConfirmAccept(Data, EConFirmType::Confirm_Friend);
			// 		}
			// 		break;
			// 	case E_WS_SSE::AcceptAdd:
			// 		break;
			// 	case E_WS_SSE::Invited:
			// 		if (MainMenuHUD != nullptr)
			// 		{
			// 			MainMenuHUD->ShowConfirmAccept(Data, EConFirmType::Confirm_Match);
			// 		}
			// 		break;
			// 	case E_WS_SSE::InviteFriendSuccess:
			// 		{
			// 			//호스트가 친구가 승인할때마다 어레이에 담아서 서버에 전송 3번째일경우 호스트는 제외 23... 로컬에서 방에 들어온 유저 데이터 킵
			// 			AGameModeBase* GameMode = UGameplayStatics::GetGameMode(GetWorld());
			// 			if (GameMode != nullptr)
			// 			{
			// 				AMainMenuGameMode* MainMenuGameMode = Cast<AMainMenuGameMode>(GameMode);
			// 				if (MainMenuGameMode != nullptr)
			// 				{
			// 					MainMenuGameMode->AddUser(Data);
			//
			// 					OnSentMessage(E_WS_Command::UpdateRoom, Data);
			// 				}
			// 			}
			// 		}
			// 		break;
			// 	case E_WS_SSE::RoomStatusRefresh:
			// 		{
			// 			if (MainMenuHUD != nullptr)
			// 			{
			// 				TArray<TSharedPtr<FJsonValue>> Array = JsonObject->GetArrayField("data");
			// 				TArray<FString> datas;
			// 				for (int i = 0; i < Array.Num(); i++)
			// 				{
			// 					FString dummy = Array[i]->AsString();
			// 					datas.Add(dummy);
			// 				}
			//
			// 				MainMenuHUD->ShowMatchmakingLobby(datas);
			// 			}
			// 		}
			// 		break;
			// 	case E_WS_SSE::MatchCreated:
			// 		{
			// 			//{"event":"MatchReady", "data":{"ip":"1.1.1.1", "port":"7777", "info":"test_message"}}
			// 			TSharedPtr<FJsonObject> JsonObject2 = JsonObject->GetObjectField("data");
			//
			// 			FLoadData jsonData;
			// 			FJsonObjectConverter::JsonObjectToUStruct(JsonObject2.ToSharedRef(), FLoadData::StaticStruct(),
			// 			                                          &jsonData, 1, 0);
			//
			// 			FString IpAddress = jsonData.ip;
			// 			FString Port = jsonData.port;
			// 			FString MatchId = jsonData.matchId;
			// 			FString Info = jsonData.info;
			//
			// 			if (IpAddress.Len() > 0 && Port.Len() > 0)
			// 			{
			// 				FString LevelName = IpAddress + ":" + Port;
			// 				UGameplayStatics::OpenLevel(GetWorld(), FName(*LevelName), false);
			// 			}
			// 			break;
			// 		}
			// 	case E_WS_SSE::SubscribeChat:
			// 		{
			// 			TSharedPtr<FJsonObject> JsonObject2 = JsonObject->GetObjectField("data");
			// 			FString nick = JsonObject2->GetStringField("user");
			// 			FString msg = JsonObject2->GetStringField("msg");
			//
			//
			// 			UGameInstance* GameInstance = GetGameInstance();
			// 			if (GameInstance != nullptr)
			// 			{
			// 				UVRFGameInstance* VRFGameInstance = Cast<UVRFGameInstance>(GameInstance);
			// 				if (VRFGameInstance != nullptr)
			// 				{
			// 					if (Dele_ChatMsg.IsBound() == true) Dele_ChatMsg.
			// 						Execute(nick + " : " + msg);
			// 				}
			// 			}
			// 		}
			// 		break;
			// 	case E_WS_SSE::Exception:
			// 		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "WS Received Error:" + Data);
			// 		break;
			// 	}
			// }
			}
		}
	}
}

void UWebSocketGameInstanceSubSystem::OnSentServer(E_WS_Command Command, FString Value)
{
	TSharedPtr<FJsonObject> RequestObj = MakeShareable(new FJsonObject);
	FString RequestBody;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&RequestBody);

	switch (Command)
	{
	case E_WS_Command::Greet:
		{
			URestApiGameInstanceSubSystem* RAGS = GetGameInstance()->GetSubsystem<URestApiGameInstanceSubSystem>();
			if (RAGS != nullptr)
			{
				RequestObj->SetStringField("Greet", RAGS->AccessToken);

				if (FJsonSerializer::Serialize(RequestObj.ToSharedRef(), Writer))
				{
					WebSocket->Send(RequestBody);
				}
			}
		}
		break;
	case E_WS_Command::GetPlayerInfo:
		{
			URestApiGameInstanceSubSystem* RAGS = GetGameInstance()->GetSubsystem<URestApiGameInstanceSubSystem>();
			if (RAGS != nullptr)
			{
				RequestObj->SetStringField("GetPlayerInfo", RAGS->AccessToken);

				if (FJsonSerializer::Serialize(RequestObj.ToSharedRef(), Writer))
				{
					WebSocket->Send(RequestBody);
				}
			}
		}
		break;
	}
	
}

void UWebSocketGameInstanceSubSystem::OnSentServer(E_WS_Command Command, int32 Value)
{
	TSharedPtr<FJsonObject> RequestObj = MakeShareable(new FJsonObject);
	FString RequestBody;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&RequestBody);

	switch (Command)
	{
	case E_WS_Command::AddPoint:
		{
			URestApiGameInstanceSubSystem* RAGS = GetGameInstance()->GetSubsystem<URestApiGameInstanceSubSystem>();
			if (RAGS != nullptr)
			{
				UZGameInstance* GameInstance = Cast<UZGameInstance>(GetGameInstance());
				//Add data
				TSharedPtr<FJsonObject> Object = MakeShareable(new FJsonObject());
				Object->SetStringField("AccessToken", RAGS->AccessToken);
				Object->SetNumberField("Point", Value);

				//JSON Entity Add data
				RequestObj->SetObjectField("AddPoint", Object);

				if (FJsonSerializer::Serialize(RequestObj.ToSharedRef(), Writer))
				{
					WebSocket->Send(RequestBody);
					UE_LOG(LogTemp, Error, TEXT("JSON: %s"), *RequestBody);
				}
			}
		}
		break;
	case E_WS_Command::UsePoint:
		{
			URestApiGameInstanceSubSystem* RAGS = GetGameInstance()->GetSubsystem<URestApiGameInstanceSubSystem>();
			if (RAGS != nullptr)
			{
				UZGameInstance* GameInstance = Cast<UZGameInstance>(GetGameInstance());
				//Add data
				TSharedPtr<FJsonObject> Object = MakeShareable(new FJsonObject());
				Object->SetStringField("AccessToken", RAGS->AccessToken);
				Object->SetNumberField("Point", Value);

				//JSON Entity Add data
				RequestObj->SetObjectField("UsePoint", Object);

				if (FJsonSerializer::Serialize(RequestObj.ToSharedRef(), Writer))
				{
					WebSocket->Send(RequestBody);
					UE_LOG(LogTemp, Error, TEXT("JSON: %s"), *RequestBody);
				}
			}
		}
		break;
	case E_WS_Command::ChangeGender:
		{
			URestApiGameInstanceSubSystem* RAGS = GetGameInstance()->GetSubsystem<URestApiGameInstanceSubSystem>();
			if (RAGS != nullptr)
			{
				UZGameInstance* GameInstance = Cast<UZGameInstance>(GetGameInstance());
				//Add data
				TSharedPtr<FJsonObject> Object = MakeShareable(new FJsonObject());
				Object->SetStringField("AccessToken", RAGS->AccessToken);
				Object->SetNumberField("Gender", Value);

				//JSON Entity Add data
				RequestObj->SetObjectField("ChangeGender", Object);

				if (FJsonSerializer::Serialize(RequestObj.ToSharedRef(), Writer))
				{
					WebSocket->Send(RequestBody);
					UE_LOG(LogTemp, Error, TEXT("JSON: %s"), *RequestBody);
				}
			}
		}
		break;
	case E_WS_Command::ChangeBodyColor:
		{
			URestApiGameInstanceSubSystem* RAGS = GetGameInstance()->GetSubsystem<URestApiGameInstanceSubSystem>();
			if (RAGS != nullptr)
			{
				UZGameInstance* GameInstance = Cast<UZGameInstance>(GetGameInstance());
				//Add data
				TSharedPtr<FJsonObject> Object = MakeShareable(new FJsonObject());
				Object->SetStringField("AccessToken", RAGS->AccessToken);
				Object->SetNumberField("BodyColor", Value);

				//JSON Entity Add data
				RequestObj->SetObjectField("ChangeBodyColor", Object);

				if (FJsonSerializer::Serialize(RequestObj.ToSharedRef(), Writer))
				{
					WebSocket->Send(RequestBody);
					UE_LOG(LogTemp, Error, TEXT("JSON: %s"), *RequestBody);
				}
			}
		}
		break;
	case E_WS_Command::ChangeBicycle:
		{
			URestApiGameInstanceSubSystem* RAGS = GetGameInstance()->GetSubsystem<URestApiGameInstanceSubSystem>();
			if (RAGS != nullptr)
			{
				UZGameInstance* GameInstance = Cast<UZGameInstance>(GetGameInstance());
				//Add data
				TSharedPtr<FJsonObject> Object = MakeShareable(new FJsonObject());
				Object->SetStringField("AccessToken", RAGS->AccessToken);
				Object->SetNumberField("Bicycle", Value);

				//JSON Entity Add data
				RequestObj->SetObjectField("ChangeBicycle", Object);

				if (FJsonSerializer::Serialize(RequestObj.ToSharedRef(), Writer))
				{
					WebSocket->Send(RequestBody);
					UE_LOG(LogTemp, Error, TEXT("JSON: %s"), *RequestBody);
				}
			}
		}
		break;
	case E_WS_Command::ChangeContry:
		{
			URestApiGameInstanceSubSystem* RAGS = GetGameInstance()->GetSubsystem<URestApiGameInstanceSubSystem>();
			if (RAGS != nullptr)
			{
				UZGameInstance* GameInstance = Cast<UZGameInstance>(GetGameInstance());
				//Add data
				TSharedPtr<FJsonObject> Object = MakeShareable(new FJsonObject());
				Object->SetStringField("AccessToken", RAGS->AccessToken);
				Object->SetNumberField("Contry", Value);

				//JSON Entity Add data
				RequestObj->SetObjectField("ChangeContry", Object);

				if (FJsonSerializer::Serialize(RequestObj.ToSharedRef(), Writer))
				{
					WebSocket->Send(RequestBody);
					UE_LOG(LogTemp, Error, TEXT("JSON: %s"), *RequestBody);
				}
			}
		}
		break;
	case E_WS_Command::AddItem:
		{
			URestApiGameInstanceSubSystem* RAGS = GetGameInstance()->GetSubsystem<URestApiGameInstanceSubSystem>();
			if (RAGS != nullptr)
			{
				UZGameInstance* GameInstance = Cast<UZGameInstance>(GetGameInstance());
				//Add data
				TSharedPtr<FJsonObject> Object = MakeShareable(new FJsonObject());
				Object->SetStringField("AccessToken", RAGS->AccessToken);
				Object->SetNumberField("Item", Value);

				//JSON Entity Add data
				RequestObj->SetObjectField("AddItem", Object);

				if (FJsonSerializer::Serialize(RequestObj.ToSharedRef(), Writer))
				{
					WebSocket->Send(RequestBody);
					UE_LOG(LogTemp, Error, TEXT("JSON: %s"), *RequestBody);
				}
			}
		}
		break;
	}
}

void UWebSocketGameInstanceSubSystem::OnSentServer(E_WS_Command Command, TArray<int32> Value)
{
	TSharedPtr<FJsonObject> RequestObj = MakeShareable(new FJsonObject);
	FString RequestBody;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&RequestBody);

	switch (Command)
	{
	case E_WS_Command::ChangePlayerEquipment:
		{
			// for (int i=0; i< Nums.Num(); i++)
			// {
			// 	UE_LOG(LogTemp,Warning,TEXT("Num [%d]: %d"), i, Nums[i]);
			// }
			URestApiGameInstanceSubSystem* RAGS = GetGameInstance()->GetSubsystem<URestApiGameInstanceSubSystem>();
			if (RAGS != nullptr)
			{
				UZGameInstance* GameInstance = Cast<UZGameInstance>(GetGameInstance());


				//Add data
				TSharedPtr<FJsonObject> Data = MakeShareable(new FJsonObject());
				//Object->SetStringField("AccessToken", RAGS->AccessToken);
				Data->SetNumberField("FrameIndex", Value[0]);
				Data->SetNumberField("WheelIndex", Value[1]);
				Data->SetNumberField("HeadgearIndex",Value[2]);
				Data->SetNumberField("GlassesIndex", Value[3]);
				Data->SetNumberField("JerseyIndex", Value[4]);
				Data->SetNumberField("GlovesIndex", Value[5]);
				Data->SetNumberField("SocksIndex", Value[6]);
				Data->SetNumberField("ShoesIndex", Value[7]);
				//JSON Entity Add data
				RequestObj->SetStringField("event", "ChangePlayerEquipment");
				RequestObj->SetObjectField("data", Data);

				if (FJsonSerializer::Serialize(RequestObj.ToSharedRef(), Writer))
				{
					WebSocket->Send(RequestBody);
					UE_LOG(LogTemp, Error, TEXT("JSON: %s"), *RequestBody);
				}
			}
		}
		break;
	case E_WS_Command::AddExp:
		{
			URestApiGameInstanceSubSystem* RAGS = GetGameInstance()->GetSubsystem<URestApiGameInstanceSubSystem>();
			if (RAGS != nullptr)
			{
				UZGameInstance* GameInstance = Cast<UZGameInstance>(GetGameInstance());
				//Add data
				TSharedPtr<FJsonObject> Object = MakeShareable(new FJsonObject());
				Object->SetStringField("AccessToken", RAGS->AccessToken);
				Object->SetNumberField("Level", Value[0]);
				Object->SetNumberField("Exp", Value[1]);

				//JSON Entity Add data
				RequestObj->SetObjectField("AddExp", Object);

				if (FJsonSerializer::Serialize(RequestObj.ToSharedRef(), Writer))
				{
					WebSocket->Send(RequestBody);
					UE_LOG(LogTemp, Error, TEXT("JSON: %s"), *RequestBody);
				}
			}
		}
		break;
	}
}

FString UWebSocketGameInstanceSubSystem::CommandConvertEnumToString(E_WS_Command Command)
{
	FString returnStr = GetEnumerationToFullString(Command);

	return returnStr;
}

FString UWebSocketGameInstanceSubSystem::SSEConvertEnumToString(E_WS_SSE ServerEvent)
{
	FString returnStr = GetEnumerationToFullString(ServerEvent);

	return returnStr;
}

E_WS_SSE UWebSocketGameInstanceSubSystem::GetSSEEnum(FString parseData)
{
	const UEnum* SSE_Enum = UClass::TryFindTypeSlow<UEnum>("/Script/ProjectZ.E_WS_SSE");

	int32 index = SSE_Enum->GetIndexByNameString(parseData);
	E_WS_SSE ResponseEnum = E_WS_SSE((uint8)index);

	return ResponseEnum;
}
