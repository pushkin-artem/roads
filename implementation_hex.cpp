struct Hex
{
    const int q;
    const int r;
    Hex(int q_, int r_): q(q_), r(r_)
    {
    }
    inline int q_() { return q; }
    inline int r_() { return r; }
    inline int s_() { return - q - r; }
};

struct FractionalHex
{
    const double q;
    const double r;
    const double s;
    FractionalHex(double q_, double r_, double s_): q(q_), r(r_), s(s_) {
        if (round(q + r + s) != 0) throw "q + r + s must be 0";
    }
};

bool operator == (Hex a, Hex b) {
    return a.q == b.q && a.r == b.r; //&& a.s == b.s;
}

bool operator != (Hex a, Hex b) {
    return !(a == b);
}

//Coordinate arithmetic
Hex hex_add(Hex a, Hex b)
{
    return Hex(a.q + b.q, a.r + b.r);
}

Hex hex_subtract(Hex a, Hex b)
{
    return Hex(a.q - b.q, a.r - b.r);
}

Hex hex_scale(Hex a, int k)
{
    return Hex(a.q * k, a.r * k);
}

const std::vector<Hex> hex_directions = {Hex(1, 0), Hex(1, -1), Hex(0, -1), Hex(-1, 0), Hex(-1, 1), Hex(0, 1)};
Hex hex_direction(int direction)
{
    return hex_directions[direction];
}

Hex hex_neighbor(Hex hex, int direction)
{
    return hex_add(hex, hex_direction(direction));
}

const std::vector<Hex> hex_diagonals = {Hex(2, -1), Hex(1, -2), Hex(-1, -1), Hex(-2, 1), Hex(-1, 2), Hex(1, 1)};
Hex hex_diagonal_neighbor(Hex hex, int direction)
{
    return hex_add(hex, hex_diagonals[direction]);
}

int hex_length(Hex hex)
{
    return int((abs(hex.q) + abs(hex.r) + abs(hex.s_())) / 2);
}

int hex_distance(Hex a, Hex b)
{
    return hex_length(hex_subtract(a, b));
}

Hex hex_round(FractionalHex h)
{
    int qi = int(round(h.q));
    int ri = int(round(h.r));
    int si = int(round(h.s));
    double q_diff = abs(qi - h.q);
    double r_diff = abs(ri - h.r);
    double s_diff = abs(si - h.s);
    if (q_diff > r_diff && q_diff > s_diff)
    {
        qi = -ri - si;
    }
    else
        if (r_diff > s_diff)
        {
            ri = -qi - si;
        }
        else
        {
            si = -qi - ri;
        }
    return Hex(qi, ri);
}

FractionalHex hex_lerp(FractionalHex a, FractionalHex b, double t)
{
    return FractionalHex(a.q * (1.0 - t) + b.q * t, a.r * (1.0 - t) + b.r * t, a.s * (1.0 - t) + b.s * t);
}

std::vector<Hex> hex_linedraw(Hex a, Hex b)
{
    int N = hex_distance(a, b);
    FractionalHex a_nudge = FractionalHex(a.q + 1e-06, a.r + 1e-06, a.s_() - 2e-06);
    FractionalHex b_nudge = FractionalHex(b.q + 1e-06, b.r + 1e-06, b.s_() - 2e-06);
    std::vector<Hex> results = {};
    double step = 1.0 / std::max(N, 1);
    for (int i = 0; i <= N; i++)
    {
        results.push_back(hex_round(hex_lerp(a_nudge, b_nudge, step * i)));
    }
    return results;
}

namespace std {
    template <> struct hash<Hex> {
        size_t operator()(const Hex& h) const {
            hash<int> int_hash;
            size_t hq = int_hash(h.q);
            size_t hr = int_hash(h.r);
            return hq ^ (hr + 0x9e3779b9 + (hq << 6) + (hq >> 2));
        }
    };
}

void map_shapes(int map_radius)
{
    std::unordered_set<Hex> map;
    for (int q = -map_radius; q <= map_radius; q++)
    {
        int r1 = std::max(-map_radius, -q - map_radius);
        int r2 = std::min(map_radius, -q + map_radius);
        for (int r = r1; r <= r2; r++)
        {
            map.insert(Hex(q, r));
        }
    }
}

// Tests

void complain(const char* name)
{
  std::cout << "FAIL " << name << std::endl;
}

void equal_hex(const char* name, Hex a, Hex b)
{
    if (!(a.q == b.q && a.s_() == b.s_() && a.r == b.r))
    {
        complain(name);
    }
}

void equal_int(const char* name, int a, int b)
{
    if (!(a == b))
    {
        complain(name);
    }
}

void equal_hex_array(const char* name, std::vector<Hex> a, std::vector<Hex> b)
{
    equal_int(name, a.size(), b.size());
    for (int i = 0; i < a.size(); i++)
    {
        equal_hex(name, a[i], b[i]);
    }
}

void test_hex_arithmetic()
{
    equal_hex("hex_add", Hex(4, -10), hex_add(Hex(1, -3), Hex(3, -7)));
    equal_hex("hex_subtract", Hex(-2, 4), hex_subtract(Hex(1, -3), Hex(3, -7)));
}

void test_hex_direction()
{
    equal_hex("hex_direction", Hex(0, -1), hex_direction(2));
}

void test_hex_neighbor()
{
    equal_hex("hex_neighbor", Hex(1, -3), hex_neighbor(Hex(1, -2), 2));
}

void test_hex_diagonal()
{
    equal_hex("hex_diagonal", Hex(-1, -1), hex_diagonal_neighbor(Hex(1, -2), 3));
}

void test_hex_distance()
{
    equal_int("hex_distance", 7, hex_distance(Hex(3, -7), Hex(0, 0)));
}

void test_hex_round()
{
    FractionalHex a = FractionalHex(0.0, 0.0, 0.0);
    FractionalHex b = FractionalHex(1.0, -1.0, 0.0);
    FractionalHex c = FractionalHex(0.0, -1.0, 1.0);
    equal_hex("hex_round 1", Hex(5, -10), hex_round(hex_lerp(FractionalHex(0.0, 0.0, 0.0), FractionalHex(10.0, -20.0, 10.0), 0.5)));
    equal_hex("hex_round 2", hex_round(a), hex_round(hex_lerp(a, b, 0.499)));
    equal_hex("hex_round 3", hex_round(b), hex_round(hex_lerp(a, b, 0.501)));
    equal_hex("hex_round 4", hex_round(a), hex_round(FractionalHex(a.q * 0.4 + b.q * 0.3 + c.q * 0.3, a.r * 0.4 + b.r * 0.3 + c.r * 0.3, a.s * 0.4 + b.s * 0.3 + c.s * 0.3)));
    equal_hex("hex_round 5", hex_round(c), hex_round(FractionalHex(a.q * 0.3 + b.q * 0.3 + c.q * 0.4, a.r * 0.3 + b.r * 0.3 + c.r * 0.4, a.s * 0.3 + b.s * 0.3 + c.s * 0.4)));
}

void test_hex_linedraw()
{
    equal_hex_array("hex_linedraw", {Hex(0, 0), Hex(0, -1), Hex(0, -2), Hex(1, -3), Hex(1, -4), Hex(1, -5)}, hex_linedraw(Hex(0, 0), Hex(1, -5)));
}

void test_all()
{
    test_hex_arithmetic();
    test_hex_direction();
    test_hex_neighbor();
    test_hex_diagonal();
    test_hex_distance();
    test_hex_round();
    test_hex_linedraw();
}