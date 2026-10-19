# IsekaiTruck
[![영상 제목](https://img.youtube.com/vi/hW9AVIHDRXk/0.jpg)](https://www.youtube.com/watch?v=hW9AVIHDRXk)



여신의 성유물, 트럭을 운전하며, 용사 후보를 이세계로  전생시키는 게임입니다.

## 프로젝트 개요

| 항목 | 내용 |
| --- | --- |
| **프로젝트명** | 이세계용사메이커 |
| **플랫폼** | PC |
| **개발 언어** | C++ |
| **사용 기술** | Direct2D |
| **담당 작업** |  D2D기반 렌더링 엔진 설계, 파티클 기능 제안을 통한 피드백 개선, UI 및 컨텐츠 구현 |

## 구현 상세

### 1. [Data 기반 Sprite Animation 시스템](.GameEngine/Animator.h)
<img width="426" height="240" alt="제목 없는 동영상 - Clipchamp로 제작 (8)" src="https://github.com/user-attachments/assets/2ad3608a-e215-458b-8345-06dff01e37d0" /><br>
Sprite Sheet를 기반으로 사용 이미지의 데이터를 만들어 애니메이션이 가능한 하나의 [Motion](.DirectY/Sprite.h#L83) 단위로 시스템을 구현했습니다.<br>
<img width="1296" height="400" alt="image" src="https://github.com/user-attachments/assets/e2b88f7d-6f60-40a3-ac28-86cd59b2f402" />

이를 통해 캐릭터 하나당 32개의 [Sprite](.DirectY/Sprite.h#L65)가 필요하고, 반복적인 Sprite 등록 및 관리를 개선했습니다.

| 항목 | 내용 |
| --- | --- |
| [Sprite](.DirectY/Sprite.h#L65) | 화면상 출력될 이미지 단위를 정의한 Sprite 구조체 |
| [Motion](.DirectY/Sprite.h#L83) | 여러 Sprite가 모여 애니메이션이 되는 단위 클래스 |
| [팀에 제공한 ReadSpriteTxtFile](./DirectY/DirectY.h#L127) | 데이터를 읽어와 자동적으로 Motion을 생성하는 helper 함수 제공 |
| [Motion 재생](/GameEngine/Animator.cpp#L31) | Motion의 index를 바꿔 Sprite Animation을 재생 |


### 2.  [Data 기반 대화 시스템](./GameProcess/MessageManager.h)
<img width="426" height="240" alt="Video Project" src="https://github.com/user-attachments/assets/9a065c9c-dd0d-48af-bf5a-be5819ae15e5" /><br>
기획자가 제공한 대사를 코드 수정없이 반영할 수 있있도록 CSV 기반 데이터 대화 시스템을 구현했습니다.<br>

| 항목 | 내용 |
| --- | --- |
| [대화 관계를 Map으로 정의](/GameProcess/MessageManager.h#L52) | 대화가 시작되면 NPC ID를 기준으로 해당 NPC의 대사 ID 목록을 조회하고, 각 ID를 실제 대사 데이터와 매핑 |
| [Queue를 활용한 대화 ](/GameProcess/MessageManager.h#L56) | 저장된 대사는 Queue에서 꺼내, UI에 전달 및 순차 출력되도록 구현 |

### 3. 2D 파티클
<img width="426" height="240" alt="제목 없는 동영상 - Clipchamp로 제작 (7)" src="https://github.com/user-attachments/assets/93cb7fab-0adc-4280-9585-294ae78817ef" />

| 항목 | 내용 |
| --- | --- |
| [파티클 상태 정의](/main/DirectY/Paticle.h) | 고유의 생명주기를 갖고 표현되는 파티클을 하나의 타입으로 정의 |
| [파티클 관리 시스템](/main/DirectY/PaticleMaintain.h) | 파티클 클래스를 일괄 관리 및 재생 |



