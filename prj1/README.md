# PRJ1 — NVMe 호스트와 OpenSSD 펌웨어

호스트에서 NVMe 명령을 보내고 Cosmos+ OpenSSD 펌웨어가 처리하는 흐름을 다룬 과제입니다.

| 위치 | 내용 |
| --- | --- |
| `host/` | Linux NVMe passthrough 기반 테스트 프로그램과 빌드 파일 |
| `firmware/` | 제출 당시 펌웨어 C/H, Xilinx 설정과 링크 스크립트 |

원본 소스는 `archive(prj1)` 커밋에 보존돼 있습니다. 이후 커밋은 실습 장치를 명시하도록 CLI를 정리하고, 전송 버퍼 할당 실패를 처리하며, 동작 설명을 보완합니다. 샘플 PNG와 제출 보고서는 포함하지 않았습니다.

호스트의 현재 사용법은 [host/README.md](host/README.md)를 참고하세요. 펌웨어는 과제 환경의 Xilinx ARM 툴체인과 BSP/보드 설정이 필요합니다. 이 저장소의 펌웨어 폴더만으로 완전한 하드웨어 빌드 환경이 제공되는 것은 아닙니다.
