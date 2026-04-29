# Page Replacement Simulator Plan

## Date

2026-04-29

## Goal

5월의 목표는 간단한 page replacement simulator를 만들고, 몇 가지 workload를 돌려서 replacement policy별 수치를 측정하는 것이다.

이번 단계의 핵심은 복잡한 커널 구현이 아니라, replacement policy의 동작 차이를 빠르게 관찰할 수 있는 실험 환경을 만드는 것이다.

## Scope

첫 버전은 trace-driven simulator로 제한한다.

- 입력은 page access trace로 둔다
- simulator는 고정된 physical frame 수를 가진다
- 각 access마다 hit 또는 miss를 판단한다
- miss가 발생하고 빈 frame이 없으면 replacement policy에 따라 victim page를 고른다
- policy별 결과를 동일한 workload에서 비교한다

초기에는 dirty page, writeback cost, process별 isolation, NUMA, huge page 같은 요소는 제외한다.
처음부터 커널의 복잡한 요소를 넣으면 로직 검증보다 디버깅에 시간을 다 쓰게 되는데, 핵심인 "교체 알고리즘"에만 집중한다.

## Replacement Policies

최소 구현 대상은 다음 네 가지로 둔다.

- FIFO
- LRU
- Random
- Belady's MIN (Optimal)

가능하면 5월 후반에 다음 정책을 추가한다.

- Clock 또는 Second Chance
- LFU 또는 Aging 계열 policy
- workload에 따라 policy를 바꾸는 간단한 adaptive policy

첫 단계에서는 정책을 많이 늘리는 것보다, 같은 trace에서 일관된 방식으로 측정하고 비교할 수 있게 만드는 것이 더 중요하다.
Belady's MIN을 통해 기존 정책들이 최적 성능과 얼마나 차이가 나는지 비교한다.


## Workloads

워크로드는 synthetic trace부터 시작한다.

### Sequential Scan

- 페이지를 0부터 N까지 순서대로 접근한다
- cache보다 큰 working set에서 policy가 어떻게 동작하는지 확인한다
- LRU가 scan workload에서 어떤 약점을 보이는지 관찰한다

### Looping Working Set

- frame 수보다 작거나 비슷한 working set을 반복 접근한다
- 정상적인 locality가 있을 때 hit ratio가 올라가는지 확인한다
- simulator 기본 동작 검증에 사용한다

### Random Access

- 넓은 주소 범위에서 무작위 page를 접근한다
- locality가 약한 workload에서 policy 간 차이가 작아지는지 확인한다

### Mixed Workload

- 반복 접근 구간과 sequential scan 구간을 섞는다
- real workload에 가까운 패턴을 흉내낸다
- adaptive policy를 실험할 때 후보 workload로 사용한다

## Metrics

기본 측정 지표는 다음과 같다.

- total accesses
- hits
- misses
- hit ratio
- miss ratio
- eviction count
- policy runtime 또는 simulation time

추가로 가능하면 다음 값을 기록한다.

- frame count별 hit ratio 변화
- workload별 policy 순위
- policy별 metadata overhead 추정
- access pattern 변화 구간에서의 miss burst

## Implementation Plan

### Week 1

- simulator 입출력 형식 결정
- trace parser 작성
- FIFO, LRU, Random 구현
- 작은 synthetic trace로 correctness test 작성

### Week 2

- workload generator 작성
- sequential, loop, random workload 생성
- frame count를 바꿔가며 batch run 가능하게 만들기
- 결과를 CSV 또는 Markdown table로 저장

### Week 3

- Clock 또는 Second Chance 구현
- workload별 결과 비교
- 간단한 plot 생성
- baseline policy 간 차이 정리

### Week 4

- mixed workload 실험
- adaptive policy 아이디어 1개 구현 여부 결정
- 5월 결과 요약 문서 작성
- 다음 단계로 ML feature와 label을 어떻게 만들지 정리

## Expected Output

5월 말까지 남길 산출물은 다음과 같다.

- 간단한 page replacement simulator
- synthetic workload generator
- policy별 측정 결과 CSV
- workload별 비교 plot
- 실험 결과를 요약한 lab diary 문서

## Research Link

이 작업은 3월 28일에 정리한 memory replacement ML 방향의 전 단계다.

ML을 바로 적용하기 전에 먼저 baseline policy가 workload별로 어떤 차이를 보이는지 측정해야 한다. 그래야 나중에 ML이 무엇을 개선해야 하는지, 어떤 feature와 label이 필요한지 더 명확히 정의할 수 있다.

동시에 여러 논문들을 읽으면서 연구의 가닥을 어떻게 잡을 것인지 고민하고 선택할 것이다.

## Immediate Next Actions

- simulator 디렉터리 위치 정하기
- trace format 초안 작성
- FIFO와 LRU부터 구현하기
- 작은 trace 2-3개로 expected hit/miss 결과를 손으로 계산해 테스트로 고정하기

## Directory Structure

초기 구조는 C++ 구현, 입력 trace, 실험 설정, 결과물, 연구 기록을 분리하는 방식으로 둔다.
한 달 동안 기능을 조금씩 채워 넣더라도 파일 위치가 흔들리지 않게 하는 것이 목적이다.

```text
replacement-sim-2026-05/
├── .gitignore
├── CMakeLists.txt
├── plan.md
├── include/
│   └── replacement_sim/
│       └── .gitkeep
├── src/
│   └── .gitkeep
├── tests/
│   └── .gitkeep
├── tools/
│   └── .gitkeep
├── traces/
│   ├── .gitkeep
│   └── synthetic/
│       └── .gitkeep
├── experiments/
│   ├── .gitkeep
│   └── configs/
│       └── .gitkeep
├── results/
│   ├── .gitkeep
│   ├── csv/
│   │   └── .gitkeep
│   └── plots/
│       └── .gitkeep
├── notebooks/
│   └── .gitkeep
├── docs/
│   ├── .gitkeep
│   └── lab-diary/
│       └── .gitkeep
```

각 디렉터리의 역할은 다음과 같이 둔다.

- `CMakeLists.txt`: C++20 기준의 최소 CMake 설정을 둔다. 처음에는 타깃 없이 시작하고, simulator 본체와 tool을 만들 때 `add_library`, `add_executable`, `add_test`를 추가한다.
- `include/replacement_sim/`: C++ 공개 헤더를 둔다. policy interface, simulator API, trace 자료구조처럼 여러 `.cpp`에서 공유할 선언이 들어간다.
- `src/`: C++ 구현 파일을 둔다. trace parser, frame table, FIFO/LRU/Random/MIN 구현, simulator 실행 로직이 여기에 들어간다.
- `tests/`: 작은 trace에 대해 손으로 계산한 expected hit/miss 결과를 C++ 테스트로 고정한다.
- `tools/`: trace generator, batch runner, result converter 같은 보조 C++ 실행 파일 또는 실험용 유틸리티를 둔다.
- `traces/`: simulator 입력으로 사용할 page access trace를 보관한다.
- `traces/synthetic/`: sequential scan, looping working set, random access, mixed workload 같은 생성 trace를 둔다.
- `experiments/`: 여러 policy와 frame count 조합을 실행하는 실험 단위를 관리한다.
- `experiments/configs/`: 반복 실행할 실험 조건을 설정 파일로 저장한다.
- `results/csv/`: policy별 측정 결과를 CSV 형태로 저장한다.
- `results/plots/`: hit ratio, miss ratio, frame count 변화 그래프를 저장한다.
- `notebooks/`: 결과를 빠르게 확인하거나 plot을 실험적으로 만들어보는 분석 노트북을 둔다.
- `docs/lab-diary/`: 매주 실험 내용, 관찰한 점, 다음 결정 사항을 연구 일지로 기록한다.
- `.gitignore`: C++ 빌드 디렉터리와 object/binary 파일, 에디터 임시 파일이 Git에 들어가지 않게 한다.
