# replacement-sim-2026-05
2026-05-페이지 교체 시뮬레이터

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