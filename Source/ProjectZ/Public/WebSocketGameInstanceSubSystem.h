// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <hlslcc/hlslcc/src/hlslcc_lib/compiler.h>

#include "CoreMinimal.h"
#include "IWebSocket.h"
#include "Commons/Commons.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "WebSocketGameInstanceSubSystem.generated.h"

///
///Player -> Server 웹소켓 명령
///
UENUM()
enum class E_WS_Command : uint8
{
	None =0,
	Greet UMETA(DisplayName ="Greet"),
	AddFriend UMETA(DisplayName = "AddFriend"),
	AcceptAdd UMETA(DisplayName = "AcceptAdd"),
	InviteFriend UMETA(DisplayName = "InviteFriend"),
	AcceptInvite UMETA(DisplayName = "AcceptInvite"),
	UpdateRoom UMETA(DisplayName = "UpdateRoom"),
	WaitQueue UMETA(DisplayName = "WaitQueue"),
	PublishChat UMETA(DisplayName = "PublishChat"),
	ChangePlayerEquipment UMETA(DisplayName = "ChangePlayerEquipmentInfo"),
	AddExp UMETA(DisplayName = "AddExp"),
	AddPoint UMETA(DisplayName = "AddPoint"),
	UsePoint UMETA(DisplayName = "UsePoint"),
	ChangeGender UMETA(DisplayName = "ChangeGender"),
	ChangeBodyColor UMETA(DisplayName = "ChangeBodyColor"),
	ChangeBicycle UMETA(DisplayName = "ChangeBicycle"),
	AddItem UMETA(DisplayName = "AddItem"),
	ChangeContry UMETA(DisplayName = "ChangeContry"),
	GetPlayerInfo UMETA(DisplayName = "GetPlayerInfo"),
};

///
///Server->Player Server Sending Event
///
UENUM()
enum class E_WS_SSE : uint8
{
	None =0,
	Registered UMETA(DisplayName ="Registered"),
	LoginAccepted UMETA(DisplayName ="LoginAccepted"),
	Added UMETA(DisplayName = "Added"),
	AcceptAdd UMETA(DisplayName = "AcceptAdd"),
	Invited UMETA(DisplayName = "Invited"),
	AcceptInvite UMETA(DisplayName = "AcceptInvite"),
	InviteFriendSuccess UMETA(DisplayName = "InviteFriendSuccess"),
	RoomStatusRefresh UMETA(DisplayName = "RoomStatusRefresh"),
	MatchCreated UMETA(DisplayName = "MatchCreated"),
	SubscribeChat UMETA(DisplayName = "SubscribeChat"),
	UpdatePlayerEquipment UMETA(DisplayName = "UpdatePlayerEquipment"),
	UpdateExp UMETA(DisplayName = "UpdateExp"),
	UpdatePoint UMETA(DisplayName = "UpdatePoint"),
	UpdateGender UMETA(DisplayName = "UpdateGender"),
	UpdateBodyColor UMETA(DisplayName = "UpdateBodyColor"),
	UpdateBicycle UMETA(DisplayName = "UpdateBicycle"),
	UpdateItem UMETA(DisplayName = "UpdateItem"),
	UpdateContry UMETA(DisplayName = "UpdateContry"),
	ReturnPlayerInfo UMETA(DisplayName = "ReturnPlayerInfo"),
	Exception UMETA(DisplayName = "Exception")
};

/*
 * 매치메이킹 구조체
 */
USTRUCT(BlueprintType)
struct FLoadData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="LoadData")
	FString ip;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="LoadData")
	FString port;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="LoadData")
	FString matchId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="LoadData")
	FString info;
};

///
///로그인 후 이름 세팅, 채팅 메시지 이벤트
///
DECLARE_DELEGATE_OneParam(FDele_LoginCompleteUsernameSetting_OneParam, FString);
DECLARE_DELEGATE_OneParam(FDele_ChatMsg, FString);


/**
 * 
 */
UCLASS()
class PROJECTZ_API UWebSocketGameInstanceSubSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UWebSocketGameInstanceSubSystem();

	/**
	 * @brief 소켓세팅
	 */
	void SetupWS();

	FDele_LoginCompleteUsernameSetting_OneParam Dele_LoginCompleteUsernameSetting_OneParam;
	FDele_ChatMsg Dele_ChatMsg;

	/**
	 * @brief 웹소켓 모듈변수
	 */
	TSharedPtr<IWebSocket> WebSocket;

	///
	///웹소켓 URL
	///
	UPROPERTY()
	FString WebSocketUrl;


	UPROPERTY()
	FString Username;

	UPROPERTY()
	FString UserContry;

	//테스트 더미데이터 JSON
	UPROPERTY()
	FString RegisteredJSON;
	
	UPROPERTY()
	FString UpdatePlayerEquipmentJSON;
	///
	///초기화, 초기화 해제
	///
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	/*
	 *서버 이벤트 수신
	 */
	UFUNCTION()
	void OnReceived(const FString& MessageString);

	/*
	 *서버에 메시지 전달
	 */
	template <typename T>
	UFUNCTION(BlueprintCallable, Category="SendMessage")
	FORCEINLINE void OnSentMessage(E_WS_Command Command, T Value)
	{
		OnSentServer(Command, Value);
	}
	
	void OnSentServer(E_WS_Command Command, FString Value);
	void OnSentServer(E_WS_Command Command, int32 Value);
	void OnSentServer(E_WS_Command Command, TArray<int32> Value);
	
	UFUNCTION()
	FString CommandConvertEnumToString(E_WS_Command Command);

	UFUNCTION()
	FString SSEConvertEnumToString(E_WS_SSE ServerEvent);

	UFUNCTION()
	E_WS_SSE GetSSEEnum(FString parseData);
};
