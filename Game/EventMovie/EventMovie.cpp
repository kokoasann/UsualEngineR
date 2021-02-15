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

	//init map
	for (int i = 0; i < m_eventMovieMarker.size(); i++) {
		m_eventMap.insert(std::make_pair(i, false));
	}
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

	for (int i = 0; i < m_eventMovieMarker.size(); i++) {
		const float EventTime = m_eventMovieMarker[i].time;
		if (m_timer >= EventTime and !m_eventMap[i]) {
			m_eventListennerFunc(m_eventMovieMarker[i].markerName);
			m_eventMap[i] = true;
			printf("markerName : ");
			printf(m_eventMovieMarker[i].markerName.c_str());
			printf("\n");
		}
	}

	if (m_frames.size() > m_frameIndex) {
		const float Time = m_frames[m_frameIndex].time;
		if (m_timer >= Time) {
			//printf("frame : %f\n", Time);
			//camera
			m_camera->SetPosition(m_frames[m_frameIndex].camMat.GetTransrate() * 1.f);
			Vector3 target = Vector3(0, 0,-1);
			m_frames[m_frameIndex].camMat.GetRotate().Apply(target);
			m_camera->SetTarget(target + m_frames[m_frameIndex].camMat.GetTransrate());

			const int numObj = m_actors.size();
			for (int i = 0; i < numObj; i++) {
				m_actors[i]->SetPosition(m_frames[m_frameIndex].objMats[i].GetTransrate());
				m_actors[i]->SetRotation(m_frames[m_frameIndex].objMats[i].GetRotate());
				m_actors[i]->SetScale(m_frames[m_frameIndex].objMats[i].GetScale());
			}
			m_frameIndex++;
		}
	}

	/*if(g_pad[0]->IsPress(enButtonA)){
		m_timer += gameTime()->GetDeltaTime() * 5;
	}*/

	m_timer += gameTime()->GetDeltaTime();

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

