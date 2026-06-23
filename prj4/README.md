# PRJ4 — Cosmos+ OpenSSD와 Linux NVMe 호환

`patches/cosmos-openssd-no-discard.patch`는 세 제출 후보에서 동일했던 커널 패치입니다. 원본 패치의 내용과 줄바꿈을 유지했습니다. 수업의 기준 커널은 Linux **5.15.190**입니다.

## 패치가 다루는 부분

- `drivers/nvme/host/core.c`: Cosmos+ 컨트롤러를 식별하고 discard 관련 동작에 quirk를 적용합니다.
- `drivers/nvme/host/nvme.h`: 해당 quirk 플래그를 정의합니다.

수업에서는 OpenSSD에서 파일 시스템을 생성할 때의 호환 문제를 다뤘습니다. 이 폴더는 제출 패치의 보관과 설명을 목적으로 하며, 전체 커널 트리나 벤치마크 도구를 포함하지 않습니다.

## 확인 범위

세 제출 패치의 SHA-256이 같음을 확인했습니다. 이번 정리에서 커널 빌드·설치·부팅이나 실제 장치 시험은 하지 않았습니다. 실제 적용은 기준 커널과 패치 충돌 여부를 확인한 별도 실습 환경에서 수행합니다.
