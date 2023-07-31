// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HttpModule.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "RestApiGameInstanceSubSystem.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTZ_API URestApiGameInstanceSubSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	URestApiGameInstanceSubSystem();

public:
	/**
	 * 초기화
	 * @brief 
	 * @param Collection 
	 */
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	/**
	 *초기화 해제
	 * @brief 
	 */
	virtual void Deinitialize() override;

	/**
	 * @brief 웹모듈
	 */
	FHttpModule* HttpModule;

	/*
	 *AWS 엑서스토큰, ID토큰, 리플레시 토큰 수집, 매치메이킹 ID
	 *
	 */
	UPROPERTY()
	FString AccessToken;

	UPROPERTY()
	FString IdToken;

	UPROPERTY()
	FString RefreshToken;

	UPROPERTY()
	FString MatchMakingTicketId;


	
	///
	///txt파일 문자열
	///
	UPROPERTY()
	FString ApiUrl;

	UPROPERTY()
	FString RegionCode;

	///
	///토큰설정
	///
	UFUNCTION()
	void SetCognitoToken(FString AT, FString IT, FString RT);

	///
	///신규 토큰 핸들러
	///
	UPROPERTY()
	FTimerHandle RetrieveNewTokensHandle;

	///
	///응답시간핸들러
	///
	UPROPERTY()
	FTimerHandle GetResponseTimerHandle;

	TDoubleLinkedList<float> PlayerLatencies;

private:
	UFUNCTION()
	void RetrieveNewTokens();

	UFUNCTION()
	void GetResponseTime();

	/**
	 * @brief AWS 서버에 전송
	 * @param Request 
	 * @param Response 
	 * @param bWasSuccessful 
	 */
	void OnRetriveNewTokensResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	void OnGetResponseTimeResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
};
