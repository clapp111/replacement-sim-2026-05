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