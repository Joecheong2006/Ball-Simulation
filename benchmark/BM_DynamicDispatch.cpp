#include <benchmark/benchmark.h>

#include <random>
#include <vector>

#include "glm/glm.hpp"
#include "TaggedPointer.h"

constexpr float PI = 3.14159265358979323846f;


struct CircleTagged {
    float radius{};
    float getArea() const { return PI * radius * radius; }
};

struct SquareTagged {
    float x, y;
    float getArea() const { return x * y; }
};

struct ShapeTagged : public TaggedPointer<CircleTagged, SquareTagged> {
    using TaggedPointer::TaggedPointer;
    float getArea() const {
        return Dispatch([](auto *obj) {
                    return obj->getArea();
                });
    }
};

struct ShapeVirtual {
    ~ShapeVirtual() = default;
    virtual float getArea() const = 0;
};

struct CircleVirtual : public ShapeVirtual {
    CircleVirtual(float r): radius(r) {}
    float radius{};
    virtual float getArea() const override { return PI * radius * radius; }
};

struct SquareVirtual : public ShapeVirtual {
    SquareVirtual(float x, float y): x(x), y(y) {}
    float x, y;
    virtual float getArea() const override { return x * y; }
};

static void BM_DynamicDispatch_TaggedPointer(benchmark::State& state) {
    auto size = state.range(0);
    std::vector<ShapeTagged> tagged_shapes;
    tagged_shapes.reserve(size);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(0.0f, 1.0f);

    for (auto i = 0; i < size; ++i)
        tagged_shapes.emplace_back(new CircleTagged{dis(gen)});

    for (auto _ : state) {
        for (auto &shape : tagged_shapes) {
            benchmark::DoNotOptimize(shape.getArea());
        }
        benchmark::DoNotOptimize(tagged_shapes);
        benchmark::ClobberMemory();
    }

    state.SetComplexityN(size);
}

static void BM_DynamicDispatch_Virtual(benchmark::State& state) {
    auto size = state.range(0);
    std::vector<ShapeVirtual*> virtual_shapes;
    virtual_shapes.reserve(size);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(0.0f, 1.0f);

    for (auto i = 0; i < size; ++i)
        virtual_shapes.emplace_back(new CircleTagged{dis(gen)});

    for (auto _ : state) {
        for (auto &shape : virtual_shapes) {
            benchmark::DoNotOptimize(shape->getArea());
        }
        benchmark::DoNotOptimize(virtual_shapes);
        benchmark::ClobberMemory();
    }

    state.SetComplexityN(size);
}

BENCHMARK(BM_DynamicDispatch_TaggedPointer)
    ->RangeMultiplier(2)
    ->Range(1 << 10, 1 << 20)
    ->Complexity();

BENCHMARK(BM_DynamicDispatch_Virtual)
    ->RangeMultiplier(2)
    ->Range(1 << 10, 1 << 20)
    ->Complexity();
