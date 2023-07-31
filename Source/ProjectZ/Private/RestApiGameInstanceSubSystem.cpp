// Fill out your copyright notice in the Description page of Project Settings.


#include "RestApiGameInstanceSubSystem.h"

#include "TextReaderComponent.h"
#include "Interfaces/IHttpResponse.h"

URestApiGameInstanceSubSystem::URestApiGameInstanceSubSystem()
{
	// txt파일을 읽어와서 해당 String 변수에 세팅 
	UTextReaderComponent* TextReader = CreateDefaultSubobject<UTextReaderComponent>(TEXT("TextReaderComp"));

	ApiUrl = TextReader->ReadFile("Urls/ApiUrl.txt");
	RegionCode = TextReader->ReadFile("Urls/RegionCode.txt");

	//Http 통신을 위한 모듈 받아오기
	HttpModule = &FHttpModule::Get();
}

void URestApiGameInstanceSubSystem::Initialize(FSubsystemCollectionBase& Collection)
{

	Super::Initialize(Collection);
}

void URestApiGameInstanceSubSystem::Deinitialize()
{

	Super::Deinitialize();

	GetWorld()->GetTimerManager().ClearTimer(RetrieveNewTokensHandle);
	GetWorld()->GetTimerManager().ClearTimer(GetResponseTimerHandle);

	//Shutdown시 토큰 소멸
	if(AccessToken.Len() > 0)
	{
		if(MatchMakingTicketId.Len() > 0)
		{
			//생성된 매치메이킹티켓으로 stopmatchmaking event call (AWS)
			TSharedPtr<FJsonObject> RequestObj = MakeShareable(new FJsonObject);
			RequestObj->SetStringField("ticketId", MatchMakingTicketId);

			FString RequestBody;
			TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&RequestBody);
			if(FJsonSerializer::Serialize(RequestObj.ToSharedRef(), Writer))
			{
				TSharedRef<IHttpRequest> StopMatchmakingRequest = HttpModule->CreateRequest();
				StopMatchmakingRequest->SetURL(ApiUrl + "/stopmatchmaking");
				StopMatchmakingRequest->SetVerb("POST");
				StopMatchmakingRequest->SetHeader("Content-Type", "application/json");
				StopMatchmakingRequest->SetHeader("Authrization", AccessToken);
				StopMatchmakingRequest->SetContentAsString(RequestBody);
				StopMatchmakingRequest->ProcessRequest();
			}
		}

		//토큰 무효화 처리
		TSharedRef<IHttpRequest> InvalidateTokensRequest = HttpModule->CreateRequest();
		InvalidateTokensRequest->SetURL(ApiUrl + "/invalidatetokens");
		InvalidateTokensRequest->SetVerb("GET");
		InvalidateTokensRequest->SetHeader("Authrization", AccessToken);
		InvalidateTokensRequest->ProcessRequest();
	}
}

void URestApiGameInstanceSubSystem::SetCognitoToken(FString AT, FString IT, FString RT)
{
	AccessToken = AT;
	IdToken = IT;
	RefreshToken = RT;
	
	GetWorld()->GetTimerManager().SetTimer(RetrieveNewTokensHandle, this, &URestApiGameInstanceSubSystem::RetrieveNewTokens, 1.0f, false, 3300.f);
}

void URestApiGameInstanceSubSystem::RetrieveNewTokens()
{
	//리플레시 토큰을 보내 새로운 토큰으로 갱신요청 승인되면 OnRetriveNewTokensResponseReceived 호출
	TSharedPtr<FJsonObject> RequestObj = MakeShareable(new FJsonObject);
	RequestObj->SetStringField("refreshToken", RefreshToken);

	FString RequestBody;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&RequestBody);

	if(FJsonSerializer::Serialize(RequestObj.ToSharedRef(), Writer))
	{
		TSharedRef<IHttpRequest> RetrieveNewTokensRequest = HttpModule->CreateRequest();
		RetrieveNewTokensRequest->OnProcessRequestComplete().BindUObject(this, &URestApiGameInstanceSubSystem::OnRetriveNewTokensResponseReceived);
		RetrieveNewTokensRequest->SetURL(ApiUrl + "/retrievenewtokens");
		RetrieveNewTokensRequest->SetVerb("POST");
		RetrieveNewTokensRequest->SetHeader("Content-Type", "application/json");
		RetrieveNewTokensRequest->SetHeader("Authrization", AccessToken);
		RetrieveNewTokensRequest->SetContentAsString(RequestBody);
		RetrieveNewTokensRequest->ProcessRequest();
	}
	else
	{
		GetWorld()->GetTimerManager().SetTimer(RetrieveNewTokensHandle, this, &URestApiGameInstanceSubSystem::RetrieveNewTokens, 1.0f, false, 30.0f);
	}
}

void URestApiGameInstanceSubSystem::GetResponseTime()
{
	//해당 레기온에 응답시간 요청
	TSharedRef<IHttpRequest> GetResponseTimeRequest = HttpModule->CreateRequest();
	GetResponseTimeRequest->OnProcessRequestComplete().BindUObject(this, &URestApiGameInstanceSubSystem::OnGetResponseTimeResponseReceived);
	GetResponseTimeRequest->SetURL("https://gamelift" + RegionCode + ".amazonaws.com");
	GetResponseTimeRequest->SetVerb("GET");
	GetResponseTimeRequest->ProcessRequest();
}

void URestApiGameInstanceSubSystem::OnRetriveNewTokensResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response,
	bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		TSharedPtr<FJsonObject> JsonObject;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

		if (FJsonSerializer::Deserialize(Reader, JsonObject))
		{
			if (JsonObject->HasField("accessToken") && JsonObject->HasField("idToken"))
			{
				//성공시 토큰 세팅
				SetCognitoToken(JsonObject->GetStringField("accessToken"), JsonObject->GetStringField("idToken"),
								 RefreshToken);
			}
		}
		else
		{
			GetWorld()->GetTimerManager().SetTimer(RetrieveNewTokensHandle, this, &URestApiGameInstanceSubSystem::RetrieveNewTokens,
												   1.0f, false, 30.0f);
		}
	}
	else
	{
		GetWorld()->GetTimerManager().SetTimer(RetrieveNewTokensHandle, this, &URestApiGameInstanceSubSystem::RetrieveNewTokens,
											   1.0f, false, 30.0f);
	}
}

void URestApiGameInstanceSubSystem::OnGetResponseTimeResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response,
	bool bWasSuccessful)
{
	// ㅁㅁㅁㅁ 머리값을 날리면 ㅁㅁㅁ에 꼬리값을 붙이면 ㅁㅁㅁㅁ
	if(PlayerLatencies.Num() >= 4)
	{
		PlayerLatencies.RemoveNode(PlayerLatencies.GetHead());
	}

	float ResponseTime = Request->GetElapsedTime() * 1000;

	PlayerLatencies.AddTail(ResponseTime);
}
