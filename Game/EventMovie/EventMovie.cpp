#include "stdafx.h"
#include "EventMovie.h"


EventMovie::EventMovie()
{

}

EventMovie::~EventMovie()
{

}



void EventMovie::Release()
{
	free(m_objMats);
}

void EventMovie::OnDestroy()
{
	Release();
}


void EventMovie::Init(const char* path, Camera* camera, const ActorInitFunc& actorInitFunc, const EventListennerFunc& eventListennerFunc)
{
	auto f = fopen(path, "rb");

	int markerNum;
	fread(&markerNum, sizeof(int), 1, f);
	m_eventMovieMarker.resize(markerNum);
	for (int i = 0; i < markerNum; i++)
	{
		auto ev = &m_eventMovieMarker[i];
		fread(&ev->time, sizeof(float), 1, f);

		int nameLen;
		fread(&nameLen, sizeof(int), 1, f);
		ev->markerName.resize(nameLen+1);
		fread(&ev->markerName.front(),sizeof(char), nameLen + 1, f);
	}

	int objNum;
	fread(&objNum, sizeof(int), 1, f);
	std::vector<SEventMovieObject> objs;
	objs.resize(objNum);
	for (int i = 0; i < objNum; i++)
	{
		auto& obj = objs[i];
		fread(&obj.id, sizeof(int), 1, f);
		
		int nameLen;
		fread(&nameLen, sizeof(int), 1, f);
		obj.objName.resize(nameLen + 1);
		fread(&obj.objName.front(), sizeof(char), nameLen + 1, f);
	}

	int frameNum;
	fread(&frameNum, sizeof(int), 1, f);
	m_objMats = (Matrix*)calloc(frameNum * objNum, sizeof(Matrix));
	m_frames.resize(frameNum);
	for (int i=0;i<frameNum;i++)
	{
		auto& frame = m_frames[i];
		fread(&frame.time, sizeof(float), 1, f);
		
		fread(&frame.camMat, sizeof(Matrix), 1, f);

		auto frameObjMats = &m_objMats[i * objNum];
		fread(frameObjMats, sizeof(Matrix), objNum, f);
		frame.objMats = frameObjMats;
	}

	fclose(f);

	m_actors.reserve(objNum);
	m_camera = camera;
	for (const auto& obj : objs)
	{
		m_actors.push_back(actorInitFunc(obj.objName));
	}

	m_eventListennerFunc = eventListennerFunc;
}

void EventMovie::Awake()
{

}

bool EventMovie::Start()
{
	return true;
}


void EventMovie::PreUpdate()
{

}

void EventMovie::Update()
{

}

void EventMovie::PostUpdate()
{

}


void EventMovie::Render()
{

}

void EventMovie::PostRender()
{

}
