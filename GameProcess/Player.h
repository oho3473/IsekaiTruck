#pragma once
#include "ElementBase.h"
#include "Vector2.h"
#include <vector>
// test
#include <functional>
#include <string>



namespace Graphics { struct Sprite; }
namespace Graphics { class Motion; }
namespace Algorithm { class FindPosition; }

class Transform;
class AudioSource;

class Player : public ElementBase
{
public:
	Player();
	~Player();

	/// Override
	virtual void Init() override;

	virtual void FixedUpdate() override;
	virtual void Update(float deltaTime) override;
	virtual void LateUpdate() override;

	virtual void PreRender() override;
	virtual void Render(DirectY* directY) override;
	virtual void PostRender(DirectY* directY) override;

	virtual void Release() override;

	virtual ElementType GetElementType() const override;

	virtual GameObject* GetGameObject() const override;
	virtual void SetGameObject(GameObject* gameObject) override;

	/// Move
	Transform* GetMyTransform() const { return m_transform; }
	void SetMyTransform(Transform* val) { m_transform = val; }

	float GetSpeed() const { return m_speed; }
	void SetSpeed(float val) { m_speed = val; }

	float GetDegree() const { return m_degree; }
	void SetDegree(float val) { m_degree = val; }

	MathEngine::Vector2 GetDirectionVecter() const { return m_directionVector; }
	void SetDirectionVecter(MathEngine::Vector2 val) { m_directionVector = val; }

	void VectorToDirection();
	void DirectionToVector();

	bool GetStun() const { return m_isStun; }
	void SetStun(bool isStun); 

	// Render
	void SetMotion(Graphics::Motion* truckMotion) { m_truckMotion = truckMotion; }

	void SetMyPaticle(Graphics::Sprite* paticle) { m_myPaticle = paticle; }
	void SetMyPaticle2(Graphics::Sprite* paticle) { m_myPaticle2 = paticle; }

	MathEngine::Vector2 GetMoveVector() const { return m_moveVector; }
	void SetMoveVector(MathEngine::Vector2 val) { m_moveVector = val; }

	/*--------------------------------------------------------------------*/
	void SetAudioSource(AudioSource* audioSource) { m_audioSource = audioSource;}
	
	void BeCaught();

private:
	Transform* m_transform;
	AudioSource* m_audioSource;

	float m_acceleration; // 트럭 가속도
	float m_earlyAcceleration;
	float m_speed; // 트럭 속도
	const float m_minSpeed; // speed 0 에서 움직이기 시작할 때 적용할 속도
	float m_damping; // 감쇠 상수

	/// 정면 충돌 시 스턴 관련
	bool m_isStun;
	float m_stunDelay;
	float m_stunSpeed; // stun 걸렸을 때 뒤로 움직이는 속도

	/// 회전 관련 상수
	const float m_truckAngle; // 트럭 애니메이션 한 장 당 각도
	float m_rotationVelocity; // 회전 속력

	float m_degree; // 실제 각도
	float m_direction; // 스프라이트와 각도를 일치시키기 위해 truckAngle의 배수로 보정할 각도
	MathEngine::Vector2 m_directionVector; // 각도를 벡터로 
	MathEngine::Vector2 m_moveVector; // 충돌할 때 보고 있는 방향과 별개로 팅겨나갈 방향

	/// Animation
	Graphics::Motion* m_truckMotion; 

	/// 파티클 관련
	float m_deltaTime;
	float m_totalTime;
	bool m_paticleDelay;
	Graphics::Sprite* m_myPaticle;
	Graphics::Sprite* m_myPaticle2;

	/// Gyu code
	// 현재의 위치에 따라 레이어를 바꾸기 위한 함수를 만들 예정
	void FindThisPositionAndSetLayer();
	Algorithm::FindPosition* m_findPosition;
	std::vector<std::vector<int>> m_layerArr
		= std::vector<std::vector<int>>(100,std::vector<int>(100));
	int m_tileLayer;
	int m_size;
	int m_currentLayer;
};