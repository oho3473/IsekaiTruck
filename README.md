# IsekaiTruck




https://github.com/user-attachments/assets/d4c54c95-a2e0-4226-9d61-5157c9173d70

주어진 단어를 조합하여 마법을 입력하며, 제한시간안에 적을 물리치는 게임입니다.<br>
프로젝트에서 게임의 메인 루프와 타자입력처리를 구현했습니다.

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
<img width="426" height="240" alt="제목 없는 동영상 - Clipchamp로 제작 (8)" src="https://github.com/user-attachments/assets/9fc9f63a-327d-4e1f-8622-d3bd20a9f627" />
Sprite Sheet를 기반으로 사용 이미지의 데이터를 만들어 애니메이션이 가능한 하나의 [Motion](.DirectY/Sprite.h#L83) 단위로 시스템을 구현했습니다.
<img width="1296" height="400" alt="image" src="https://github.com/user-attachments/assets/e2b88f7d-6f60-40a3-ac28-86cd59b2f402" />

이를 통해 캐릭터 하나당 32개의 [Sprite](.DirectY/Sprite.h#L65)가 필요하고, 반복적인 Sprite 등록 및 관리를 개선했습니다.

| 항목 | 내용 |
| --- | --- |
| [Sprite](.DirectY/Sprite.h#L65) | 화면상 출력될 이미지 단위를 정의한 Sprite 구조체 |
| [Motion](.DirectY/Sprite.h#L83) | 여러 Sprite가 모여 애니메이션이 되는 단위 클래스 |
| [팀에 제공한 ReadSpriteTxtFile](./DirectY/DirectY.h#L127) | 데이터를 읽어와 자동적으로 Motion을 생성하는 helper 함수 제공 |


### 2.  [Data 기반 대화 시스템](./GameProcess/MessageManager.h)
<img width="426" height="240" alt="Video Project" src="https://github.com/user-attachments/assets/9a065c9c-dd0d-48af-bf5a-be5819ae15e5" />
기획자가 제공한 대사를 코드 수정없이 반영할 수 있있도록 CSV 기반 데이터 대화 시스템을 구현했습니다.

<img width="989" height="354" alt="image" src="https://github.com/user-attachments/assets/a6e32063-259d-46ff-af4e-ea0ea48a20e2" />

대화가 시작되면 NPC ID를 기준으로 해당 NPC의 대사 ID 목록을 조회하고, 각 ID를 실제 대사 데이터와 매핑
[](/GameProcess/MessageManager.h#L49)


저장된 대사는 Queue에서 꺼내, UI에 전달하여 순차 출력되도록 구현했습니다
