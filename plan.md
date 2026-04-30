# Page Replacement Simulator Plan

## Write Date

2026-04-29

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


