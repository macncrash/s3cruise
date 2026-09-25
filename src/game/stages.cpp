#include "stages.h"

#include <algorithm>
#include <cmath>

namespace rally {

constexpr uint16_t C(int r, int g, int b) { return uint16_t(((r & 15) << 8) | ((g & 15) << 4) | (b & 15)); }

const ObjInfo OBJ[O_COUNT] = {
    /* PINE      */ {2600, 0.16f, true, true, true},
    /* PINE_SNOW */ {2600, 0.16f, true, true, true},
    /* BIRCH     */ {2300, 0.12f, true, true, true},
    /* BUSH      */ {520, 0.30f, false, false, true},
    /* DRYBUSH   */ {420, 0.25f, false, false, true},
    /* ROCK      */ {450, 0.25f, true, true, true},
    /* BOULDER   */ {1300, 0.50f, true, true, true},
    /* CACTUS    */ {1600, 0.14f, true, true, true},
    /* PALM      */ {2600, 0.12f, true, true, true},
    /* REEDS     */ {520, 0.20f, false, false, true},
    /* SNOWBANK  */ {500, 0.55f, false, false, true},
    /* CLIFF     */ {2800, 0.60f, true, false, true},
    /* LOGS      */ {420, 0.40f, true, true, true},
    /* CHEVRON   */ {720, 0.32f, true, true, false},
    /* CROWD     */ {820, 0.50f, false, false, false},
    /* FLAG      */ {1500, 0.08f, true, false, false},
    /* ARCH_START*/ {2700, 0.00f, false, false, false},
    /* ARCH_CP   */ {2700, 0.00f, false, false, false},
    /* TIRES     */ {380, 0.35f, true, true, false},
    /* BALE      */ {400, 0.35f, true, true, false},
    /* BOAT      */ {950, 0.00f, false, false, false},
};

const char* VOICE_TEXT[V_COUNT] = {
    "Easy left",   "Easy right",  "Medium left", "Medium right", "Hard left",   "Hard right",
    "Hairpin left. Don't cut!", "Hairpin right. Don't cut!", "Long easy left", "Long easy right",
    "Over crest!", "Splash!", "Don't cut!", "Checkpoint!", "Extended play!", "Final lap!",
    "Game over. Yeah!", "Congratulations!", "Three", "Two", "One", "Go!", "Finish!", "You win!", "Maybe.",
    "Eighty-eight point one. The Blade!", "One oh one point five. Neon F.M.", "Ninety-four seven. Kool rock and roll!",
    "Turbo!", "Secret unlocked. Turbo boost!", "Ninety-six point six. Arena F.M.!",
};

// ------------------------------------------------------------------ stages

// Pacific Coast Highway, late afternoon to sunset. Road palette roles:
// 1-3 ground, 4-5 kerb, 6-7 asphalt bands, 8 dark, 9 tracks, 10 ridge, 11-13 ocean, 14 paint, 15 speck.
static StageDef coast(const char* name, const char* sub, int laps, uint16_t skyTop, uint16_t skyHor, uint16_t fog,
                      float fogNear, float fogFar, const uint16_t (&ground)[3], const uint16_t (&sea)[3], int music) {
    StageDef s{};
    s.name = name;
    s.subtitle = sub;
    s.laps = laps;
    s.startTime = 36;
    s.extend = 17;
    s.skyTop = skyTop;
    s.skyHorizon = skyHor;
    s.fog = fog;
    s.fogNear = fogNear;
    s.fogFar = fogFar;
    s.backdropFog = 4;
    const uint16_t road[16] = {0, ground[0], ground[1], ground[2], C(15, 15, 15), C(14, 3, 2), C(6, 5, 7), C(5, 4, 6),
                               C(3, 3, 4), C(5, 5, 6), C(7, 6, 7), sea[0], sea[1], sea[2], C(15, 14, 10), C(8, 7, 8)};
    std::copy(road, road + 16, s.roadMain);
    std::copy(road, road + 16, s.roadTarmac);
    std::copy(road, road + 16, s.roadAlt);
    s.roadMain[6] = C(13, 11, 8); s.roadMain[7] = C(12, 10, 7);  // sand
    const uint16_t scene[16] = {0, C(1, 1, 1), C(6, 4, 3), C(9, 6, 4), C(2, 5, 2), C(4, 7, 3), C(8, 10, 4), C(15, 15, 14),
                                C(8, 6, 6), C(11, 9, 8), C(14, 12, 10), C(15, 5, 3), C(13, 10, 6), C(15, 12, 8), sea[1], C(15, 13, 6)};
    std::copy(scene, scene + 16, s.scene);
    const uint16_t far[16] = {0, C(15, 12, 10), C(14, 8, 8), C(8, 5, 8), C(6, 4, 7), C(14, 10, 10), C(11, 7, 8),
                              C(10, 6, 7), C(15, 15, 10), C(15, 12, 5), skyHor, sea[0], sea[2], 0, 0, 0};
    std::copy(far, far + 16, s.far);
    const uint16_t near[16] = {0, C(6, 6, 4), C(4, 4, 4), C(2, 3, 2), C(3, 4, 3), C(4, 6, 3), sea[0], sea[2],
                               C(13, 11, 8), C(11, 9, 7), C(7, 6, 6), C(15, 13, 8), C(2, 2, 3), 0, 0, 0};
    std::copy(near, near + 16, s.near);
    s.weather = 0;
    s.music = music;
    return s;
}

static StageDef makeDesert() {  // Santa Monica: pier, palms, the beach on your left
    return coast("SANTA MONICA", "PIER TO MALIBU", 3, C(3, 6, 13), C(15, 12, 8), C(15, 12, 9), 40, 260,
                 {C(13, 11, 7), C(12, 10, 6), C(14, 12, 9)}, {C(3, 7, 12), C(2, 6, 11), C(12, 14, 15)}, 0);
}
static StageDef makeForest() {  // Big Sur: cliffs over the Pacific, redwoods inland
    return coast("BIG SUR", "BIXBY CLIFFS", 3, C(3, 5, 11), C(15, 10, 7), C(13, 10, 9), 35, 230,
                 {C(4, 7, 3), C(3, 6, 2), C(6, 8, 4)}, {C(2, 5, 10), C(1, 4, 9), C(11, 13, 15)}, 1);
}
static StageDef makeMountain() {  // Monterey: cypress and sea fog
    return coast("MONTEREY", "CYPRESS FOG", 3, C(7, 8, 11), C(13, 12, 12), C(12, 12, 13), 18, 150,
                 {C(5, 7, 4), C(4, 6, 3), C(7, 8, 5)}, {C(4, 6, 9), C(3, 5, 8), C(12, 13, 14)}, 2);
}
static StageDef makeLakeside() {  // the Golden Gate at sunset
    return coast("GOLDEN GATE", "SUNSET INTO THE CITY", 2, C(3, 3, 9), C(15, 8, 4), C(14, 8, 6), 30, 230,
                 {C(6, 7, 4), C(5, 6, 3), C(8, 8, 5)}, {C(6, 4, 9), C(5, 3, 8), C(15, 11, 5)}, 3);
}

const StageDef& stageDef(int i) {
    static const StageDef defs[NUM_STAGES] = {makeDesert(), makeForest(), makeMountain(), makeLakeside()};
    return defs[std::clamp(i, 0, NUM_STAGES - 1)];
}

// ------------------------------------------------------------------ tracks

namespace {

struct Piece {
    int enter, hold, leave;
    float curve, hill;
    Surface surface;
    int zone;
    uint8_t left, right;
};

struct Rng {
    uint32_t a;
    float operator()() {
        a += 0x6d2b79f5u;
        uint32_t t = a;
        t = (t ^ (t >> 15)) * (1 | t);
        t ^= t + ((t ^ (t >> 7)) * (61 | t));
        return float((t ^ (t >> 14)) >> 8) / 16777216.0f;
    }
    float side() { return (*this)() < 0.5f ? -1.0f : 1.0f; }
};

float easeIn(float a, float b, float t) { return a + (b - a) * t * t; }
float easeInOut(float a, float b, float t) { return a + (b - a) * (-std::cos(t * 3.14159265f) / 2 + 0.5f); }

std::vector<Piece> layout(int stage) {
    // The coast highway is tarmac all the way; a little sand blows across it at the beach.
    constexpr Surface D = TARMAC, T = TARMAC, S = SAND, W = TARMAC, F = TARMAC;
    switch (stage) {
        case 0:  // DESERT: fast and open, big jumps, canyon, a tarmac village
            return {
                {10, 60, 10, 0, 0, D, 3},     {25, 40, 25, 2, 10, D, 0},    {20, 20, 20, 0, -10, D, 0},
                {20, 30, 20, -3, 0, D, 0},    {10, 25, 10, 0, 18, S, 0},    {10, 10, 10, 0, -18, S, 0},
                {20, 50, 20, 2.5f, 0, D, 1},  {15, 20, 15, -4.5f, 0, D, 1}, {15, 20, 15, 3.5f, 0, D, 1},
                {20, 40, 20, 0, 8, T, 2},     {15, 25, 15, 4, 0, T, 2},     {15, 25, 15, -4, 0, T, 2},
                {20, 30, 20, 0, -8, D, 0},    {20, 40, 20, -2, 12, S, 0},   {20, 20, 20, 3, -12, S, 0},
                {30, 60, 30, 0, 0, D, 0},     {15, 30, 15, -5.8f, 0, D, 1}, {10, 20, 10, 0, 0, D, 0},
                {20, 40, 20, 2, 6, D, 0},     {20, 40, 20, -2, -6, D, 0},   {20, 30, 20, 1.5f, 0, D, 0},
            };
        case 1:  // FOREST: rhythm sections, a water splash, a log bridge (tarmac)
            return {
                {10, 60, 10, 0, 0, D, 3},     {20, 30, 20, 2.5f, 6, D, 0},  {10, 20, 10, 0, -6, D, 0},
                {15, 20, 15, -4, 0, D, 0},    {15, 20, 15, 4, 8, D, 0},     {15, 15, 15, -3, -8, D, 0},
                {10, 12, 10, 0, 0, F, 1},     {20, 50, 20, 0, 18, D, 1},    {20, 20, 20, 0, -18, D, 1},
                {20, 40, 20, -6, 0, D, 0},    {10, 30, 10, 0, 0, T, 2},     {20, 60, 20, 2, 10, D, 1},
                {15, 30, 15, -2, -10, D, 1},  {15, 15, 15, 5, 0, D, 0},     {15, 15, 15, -5, 0, D, 0},
                {20, 40, 20, 0, 14, D, 0},    {20, 20, 20, 0, -14, D, 0},   {20, 50, 20, 3.5f, 6, D, 0},
                {15, 30, 15, -3, -6, D, 1},   {15, 30, 15, -5, 0, D, 0},    {15, 25, 15, 3, 0, D, 0},
                {20, 40, 20, -2, 0, D, 1},
            };
        case 2:  // MOUNTAIN: hairpins, cliffs, snow and tarmac
            return {
                {10, 50, 10, 0, 0, T, 3},     {15, 25, 15, 3, 10, T, 1},    {15, 20, 15, -5.5f, 8, T, 1},
                {10, 30, 10, 0, 10, D, 0},    {15, 20, 15, 4.5f, 6, W, 0},  {15, 20, 15, -4.5f, 0, W, 0},
                {20, 40, 20, 0, -12, D, 1},   {15, 25, 15, 6, 0, D, 1},     {10, 30, 10, 0, 12, W, 0},
                {15, 20, 15, -3.5f, 0, W, 0}, {15, 20, 15, 3.5f, -8, W, 0}, {20, 30, 20, 0, 0, T, 2},
                {15, 20, 15, -6, 0, T, 1},    {15, 30, 15, 2.5f, -10, D, 0}, {15, 20, 15, -2.5f, -10, D, 0},
                {15, 20, 15, 5, 0, W, 1},     {20, 40, 20, 0, 6, D, 0},     {15, 25, 15, -4, 0, D, 0},
                {20, 30, 20, 2, 0, T, 2},
            };
        default:  // LAKESIDE: fast shoreline, lake to the right, two fords
            return {
                {10, 60, 10, 0, 0, T, 3},     {25, 50, 25, -2, 4, D, 0, 0, 1},  {20, 40, 20, 3, -4, D, 0, 0, 1},
                {10, 10, 10, 0, 0, F, 0, 1, 1}, {20, 40, 20, -3.5f, 6, D, 1},   {15, 25, 15, 4.5f, 0, D, 1},
                {20, 60, 20, 1.5f, -6, D, 0, 0, 1}, {15, 20, 15, -5, 0, D, 0, 0, 1}, {20, 30, 20, 0, 10, T, 2},
                {20, 20, 20, 0, -10, T, 2},   {20, 50, 20, 2.5f, 0, D, 0, 0, 1}, {10, 10, 10, 0, 0, F, 0, 1, 1},
                {15, 25, 15, -4, 0, D, 1},    {20, 40, 20, 2, 0, D, 0, 0, 1},  {25, 50, 25, -1.5f, 0, D, 0, 0, 1},
            };
    }
}

void place(Track& t, Rng& rnd, int stage, const std::vector<int>& zoneOf) {
    const int N = t.N;
    auto put = [&](int i, ObjType type, float off, bool flip = false) {
        Segment& s = t.segs[((i % N) + N) % N];
        if ((off > 0 && s.right == 1 && type != O_BOAT && type != O_REEDS) ||
            (off < 0 && s.left == 1 && type != O_BOAT && type != O_REEDS))
            return;  // no trees in the lake
        s.objs.push_back({type, off, flip});
    };
    for (int i = 0; i < N; i++) {
        Segment& seg = t.segs[i];
        const int zone = zoneOf[i];
        const float r = rnd();
        switch (stage) {
            case 0:
                if (zone == 1) {  // canyon walls
                    if (i % 2 == 0) put(i, O_CLIFF, -(1.7f + rnd() * 0.4f), rnd() < 0.5f);
                    if (i % 2 == 1) put(i, O_CLIFF, 1.7f + rnd() * 0.4f, rnd() < 0.5f);
                    if (r < 0.15f) put(i, O_BOULDER, rnd.side() * (1.5f + rnd() * 0.3f));
                } else if (zone == 2 || zone == 3) {
                    if (i % 6 == 0) put(i, O_BALE, rnd.side() * 1.45f);
                    if (i % 9 == 0) put(i, O_CROWD, rnd.side() * (2.1f + rnd() * 0.8f), rnd() < 0.5f);
                    if (i % 5 == 0) put(i, O_PALM, rnd.side() * (2.2f + rnd() * 2.5f));
                } else {
                    if (i % 4 == 0) put(i, O_PALM, rnd.side() * (1.6f + rnd() * 3.0f), rnd() < 0.5f);
                    if (r < 0.14f) put(i, O_BUSH, rnd.side() * (1.4f + rnd() * 2.0f));
                    if (r > 0.95f) put(i, O_BOULDER, rnd.side() * (2.2f + rnd() * 3.0f), rnd() < 0.5f);
                    if (r > 0.88f && r < 0.92f) put(i, O_ROCK, rnd.side() * (1.5f + rnd()));
                }
                break;
            case 1:
                if (zone == 0) {
                    if (i % 3 == 0) put(i, O_PINE, -(1.5f + rnd() * 0.7f), rnd() < 0.5f);
                    if (i % 3 == 1) put(i, O_PINE, 1.5f + rnd() * 0.7f, rnd() < 0.5f);
                    if (i % 4 == 2) put(i, O_PINE, rnd.side() * (2.4f + rnd() * 3.0f));
                } else if (zone == 1) {
                    if (r < 0.14f) put(i, O_BUSH, rnd.side() * (1.4f + rnd() * 1.2f));
                    if (i % 5 == 0) put(i, O_BIRCH, rnd.side() * (1.8f + rnd() * 2.5f), rnd() < 0.5f);
                    if (i % 7 == 3) put(i, O_PINE, rnd.side() * (2.5f + rnd() * 3.0f));
                    if (r > 0.97f) put(i, O_LOGS, rnd.side() * (1.5f + rnd() * 0.5f));
                } else {
                    if (i % 6 == 0) put(i, O_BALE, rnd.side() * 1.45f);
                    if (i % 8 == 0) put(i, O_CROWD, rnd.side() * (2.0f + rnd() * 0.8f), rnd() < 0.5f);
                    if (i % 4 == 1) put(i, O_PINE, rnd.side() * (3.0f + rnd() * 2.0f));
                }
                break;
            case 2:
                if (zone == 1) {  // cliff on the inside, snowbank on the outside
                    float side = seg.curve >= 0 ? 1.0f : -1.0f;
                    if (i % 2 == 0) put(i, O_CLIFF, side * (1.6f + rnd() * 0.3f), rnd() < 0.5f);
                    if (i % 3 == 0) put(i, O_BUSH, -side * (1.35f + rnd() * 0.2f), rnd() < 0.5f);
                    if (i % 5 == 0) put(i, O_PINE, -side * (2.3f + rnd() * 2.0f));
                } else if (zone == 2 || zone == 3) {
                    if (i % 5 == 0) put(i, O_TIRES, rnd.side() * 1.4f);
                    if (i % 8 == 0) put(i, O_CROWD, rnd.side() * (2.0f + rnd() * 0.6f), rnd() < 0.5f);
                    if (i % 3 == 0) put(i, O_PINE, rnd.side() * (2.6f + rnd() * 2.5f));
                } else {
                    if (i % 3 == 0) put(i, O_PINE, -(1.5f + rnd() * 0.8f), rnd() < 0.5f);
                    if (i % 3 == 1) put(i, O_PINE, 1.5f + rnd() * 0.8f, rnd() < 0.5f);
                    if (i % 4 == 2) put(i, O_BUSH, rnd.side() * (1.3f + rnd() * 0.2f));
                    if (r > 0.96f) put(i, O_BOULDER, rnd.side() * (2.2f + rnd() * 2.0f));
                }
                break;
            default:
                if (zone == 1) {
                    if (i % 4 == 0) put(i, O_BIRCH, rnd.side() * (1.6f + rnd() * 2.5f), rnd() < 0.5f);
                    if (r < 0.15f) put(i, O_BUSH, rnd.side() * (1.4f + rnd()));
                    if (i % 6 == 2) put(i, O_PALM, rnd.side() * (2.0f + rnd() * 2.0f), rnd() < 0.5f);
                } else if (zone == 2 || zone == 3) {
                    if (i % 6 == 0) put(i, O_BALE, rnd.side() * 1.45f);
                    if (i % 8 == 0) put(i, O_CROWD, rnd.side() * (2.0f + rnd() * 0.8f), rnd() < 0.5f);
                    if (i % 5 == 0) put(i, O_PALM, rnd.side() * (2.4f + rnd() * 2.0f));
                } else {
                    if (i % 4 == 0) put(i, O_PALM, -(1.6f + rnd() * 1.6f), rnd() < 0.5f);
                    if (i % 4 == 2 && seg.right == 0) put(i, O_PALM, 1.6f + rnd() * 1.6f, rnd() < 0.5f);
                    if (seg.right == 1 && i % 3 == 0) put(i, O_REEDS, 1.3f + rnd() * 0.3f, rnd() < 0.5f);
                    if (seg.right == 1 && i % 23 == 0) put(i, O_BOAT, 4.0f + rnd() * 6.0f, rnd() < 0.5f);
                    if (r < 0.08f) put(i, O_BUSH, -(1.4f + rnd()));
                }
                break;
        }
        // Chevron boards on the outside of sharp bends, tyre walls at hairpins.
        if (std::fabs(seg.curve) > 2.2f && i % 8 == 0) {
            bool right = seg.curve > 0;
            put(i, O_CHEVRON, right ? -1.45f : 1.45f, !right);
        }
        if (std::fabs(seg.curve) > 5 && i % 3 == 0) put(i, O_TIRES, seg.curve > 0 ? -1.4f : 1.4f);
    }
    // Clean up around the gates, then add arches, crowds and flags.
    for (size_t c = 0; c < t.checkpoints.size(); c++) {
        int cp = t.checkpoints[c];
        for (int k = -10; k < 12; k++) {
            Segment& s = t.segs[((cp + k) % N + N) % N];
            s.objs.erase(std::remove_if(s.objs.begin(), s.objs.end(), [](const Obj& o) { return std::fabs(o.off) < 2.8f; }),
                         s.objs.end());
        }
        put(cp, c == 0 ? O_ARCH_START : O_ARCH_CP, 0);
        for (int k = 2; k < 10; k += 2) {
            put(cp - k * 2, O_CROWD, -2.0f - (k % 4) * 0.25f, k % 4 == 0);
            put(cp - k * 2 - 1, O_CROWD, 2.0f + (k % 4) * 0.25f, k % 4 != 0);
        }
        for (int k = 0; k < 5; k++) {
            put(cp + 3 + k * 2, O_FLAG, -1.3f);
            put(cp + 3 + k * 2, O_FLAG, 1.3f, true);
        }
    }
}

void buildMap(Track& t) {
    const int N = t.N;
    double total = 0;
    for (auto& s : t.segs) total += s.curve;
    // Scale curvature so the course closes into one loop on the map.
    double k = std::fabs(total) > 1 ? 2 * 3.14159265 / total : 0.004;
    std::vector<std::pair<double, double>> pts(N + 1);
    double h = 0, x = 0, y = 0;
    for (int i = 0; i <= N; i++) {
        pts[i] = {x, y};
        if (i == N) break;
        h += t.segs[i].curve * k;
        x += std::sin(h);
        y -= std::cos(h);
    }
    double ex = pts[N].first, ey = pts[N].second;
    double minX = 1e9, maxX = -1e9, minY = 1e9, maxY = -1e9;
    for (int i = 0; i <= N; i++) {
        pts[i].first -= ex * i / N;
        pts[i].second -= ey * i / N;
        minX = std::min(minX, pts[i].first); maxX = std::max(maxX, pts[i].first);
        minY = std::min(minY, pts[i].second); maxY = std::max(maxY, pts[i].second);
    }
    double span = std::max(maxX - minX, maxY - minY);
    t.map.resize(N);
    for (int i = 0; i < N; i++)
        t.map[i] = {float((pts[i].first - minX) / span + (1 - (maxX - minX) / span) / 2),
                    float((pts[i].second - minY) / span + (1 - (maxY - minY) / span) / 2)};
}

}  // namespace

Track buildTrack(int stage) {
    Track t;
    std::vector<int> zoneOf;
    std::vector<Piece> pieces = layout(stage);
    auto lastY = [&]() { return t.segs.empty() ? 0.0f : t.segs.back().y2; };
    auto add = [&](float curve, float y, const Piece& p) {
        Segment s;
        s.i = int(t.segs.size());
        s.z1 = s.i * SEG;
        s.z2 = (s.i + 1) * SEG;
        s.y1 = lastY();
        s.y2 = y;
        s.curve = curve;
        s.surface = p.surface;
        s.band = uint8_t((s.i / BAND) % 2);
        s.left = p.left;
        s.right = p.right;
        t.segs.push_back(std::move(s));
        zoneOf.push_back(p.zone);
    };
    auto road = [&](const Piece& p) {
        int start = int(t.segs.size());
        float y0 = lastY(), y1 = y0 + p.hill * SEG;
        int total = p.enter + p.hold + p.leave;
        for (int n = 0; n < p.enter; n++) add(easeIn(0, p.curve, float(n) / p.enter), easeInOut(y0, y1, float(n + 1) / total), p);
        for (int n = 0; n < p.hold; n++) add(p.curve, easeInOut(y0, y1, float(p.enter + n + 1) / total), p);
        for (int n = 0; n < p.leave; n++)
            add(easeInOut(p.curve, 0, float(n) / p.leave), easeInOut(y0, y1, float(p.enter + p.hold + n + 1) / total), p);
        // Pace notes.
        float ac = std::fabs(p.curve);
        int dir = p.curve > 0 ? 1 : -1;
        if (ac >= 1.4f) {
            PaceNote n;
            n.seg = start - 45;
            n.dir = dir;
            if (ac >= 5.5f) { n.icon = 3; n.voice = dir > 0 ? V_HAIR_R : V_HAIR_L; n.text = dir > 0 ? "HAIRPIN RIGHT" : "HAIRPIN LEFT"; }
            else if (ac >= 4) { n.icon = 2; n.voice = dir > 0 ? V_HARD_R : V_HARD_L; n.text = dir > 0 ? "HARD RIGHT" : "HARD LEFT"; }
            else if (ac >= 2.8f) { n.icon = 1; n.voice = dir > 0 ? V_MED_R : V_MED_L; n.text = dir > 0 ? "MEDIUM RIGHT" : "MEDIUM LEFT"; }
            else if (p.hold >= 40) { n.icon = 0; n.voice = dir > 0 ? V_LONG_R : V_LONG_L; n.text = dir > 0 ? "LONG EASY RIGHT" : "LONG EASY LEFT"; }
            else { n.icon = 0; n.voice = dir > 0 ? V_EASY_R : V_EASY_L; n.text = dir > 0 ? "EASY RIGHT" : "EASY LEFT"; }
            t.notes.push_back(n);
        }
        if (p.hill >= 14) t.notes.push_back({start + p.enter + p.hold - 25, V_CREST, 4, 0, "OVER CREST"});
        if (p.surface == FORD) t.notes.push_back({start - 40, V_SPLASH, 5, 0, "WATER SPLASH"});
    };
    for (const Piece& p : layout(stage)) road(p);
    // Return to the start height and finish with a straight.
    Piece down{30, 40, 30, 0, -lastY() / SEG, DIRT, 0};
    down.surface = stage == 2 || stage == 3 ? TARMAC : DIRT;
    road(down);
    Piece home{10, 50, 10, 0, 0, pieces[0].surface, 3};
    road(home);

    t.N = int(t.segs.size());
    t.length = t.N * SEG;
    t.checkpoints = {0, t.N / 2};
    for (auto& n : t.notes) n.seg = ((n.seg % t.N) + t.N) % t.N;
    std::sort(t.notes.begin(), t.notes.end(), [](const PaceNote& a, const PaceNote& b) { return a.seg < b.seg; });
    // Heading north up the coast, the Pacific is on the left: at the beach, along the cliffs and past the cypress.
    for (int i = 0; i < t.N; i++) {
        const int z = zoneOf[size_t(i)];
        if ((stage == 0 && z != 2) || (stage == 1 && z != 2) || (stage == 2 && z == 0)) t.segs[size_t(i)].left = 1;
    }
    Rng rnd{uint32_t(2026 + stage * 7919)};
    place(t, rnd, stage, zoneOf);
    buildMap(t);
    return t;
}

}  // namespace rally
