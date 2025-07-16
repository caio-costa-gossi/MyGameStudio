#pragma once
#include <cstdint>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

struct Vec3F;
struct Vec3I64;
struct Vec3U64;

struct Vec2U64
{
	uint64_t X;
	uint64_t Y;

	// Constructors & destructor
	Vec2U64() : X(0), Y(0) {}
	Vec2U64(const uint64_t x, const uint64_t y) : X(x), Y(y) {}
	Vec2U64(const Vec2U64& other) { X = other.X; Y = other.Y; }
	Vec2U64(Vec2U64&& other) noexcept { X = other.X; Y = other.Y; }

	~Vec2U64() = default;

	// Assignment
	Vec2U64& operator=(const Vec2U64& other) = default;
	Vec2U64& operator=(Vec2U64&& other) = default;

	// Sum & Subtraction
	Vec2U64 operator+(const Vec2U64& other) const { return { X + other.X, Y + other.Y }; }
	Vec2U64 operator-(const Vec2U64& other) const { return { X - other.X, Y - other.Y }; }
	Vec2U64 operator+=(const Vec2U64& other) { X += other.X; Y += other.Y; return *this; }
	Vec2U64 operator-=(const Vec2U64& other) { X -= other.X; Y -= other.Y; return *this; }

	// Multiplication & Division
	Vec2U64 operator*(const uint64_t scalar) const { return { X * scalar, Y * scalar }; }
	Vec2U64 operator*=(const uint64_t scalar) { X *= scalar; Y *= scalar; return *this; }
	Vec2U64 operator/(const uint64_t scalar) const { return { X / scalar, Y / scalar }; }
	Vec2U64 operator/=(const uint64_t scalar) { X /= scalar; Y /= scalar; return *this; }

	// Conversion
	operator Vec3U64() const;
};

struct Vec3U64
{
	uint64_t X;
	uint64_t Y;
	uint64_t Z;

	// Constructors & destructor
	Vec3U64() : X(0), Y(0), Z(0) { }
	Vec3U64(const uint64_t x, const uint64_t y, const uint64_t z) : X(x), Y(y), Z(z) { }
	Vec3U64(const Vec3U64& other) { X = other.X; Y = other.Y; Z = other.Z; }
	Vec3U64(Vec3U64&& other) noexcept { X = other.X; Y = other.Y; Z = other.Z; }

	~Vec3U64() = default;

	// Assignment
	Vec3U64& operator=(const Vec3U64& other) = default;
	Vec3U64& operator=(Vec3U64&& other) = default;

	// Sum & Subtraction
	Vec3U64 operator+(const Vec3U64& other) const { return { X + other.X, Y + other.Y, Z + other.Z }; }
	Vec3U64 operator-(const Vec3U64& other) const { return { X - other.X, Y - other.Y, Z - other.Z }; }
	Vec3U64 operator+=(const Vec3U64& other) { X += other.X; Y += other.Y; Z += other.Z; return *this; }
	Vec3U64 operator-=(const Vec3U64& other) { X -= other.X; Y -= other.Y; Z -= other.Z; return *this; }

	// Multiplication & Division
	Vec3U64 operator*(const uint64_t scalar) const { return { X * scalar, Y * scalar, Z * scalar }; }
	Vec3U64 operator*=(const uint64_t scalar) { X *= scalar; Y *= scalar; Z *= scalar; return *this; }
	Vec3U64 operator/(const uint64_t scalar) const { return { X / scalar, Y / scalar, Z / scalar }; }
	Vec3U64 operator/=(const uint64_t scalar) { X /= scalar; Y /= scalar; Z /= scalar; return *this; }

	// Conversion
	operator Vec2U64() const { return { X,Y }; }
};

struct Vec2I64
{
	int64_t X;
	int64_t Y;

	// Constructors & destructor
	Vec2I64() : X(0), Y(0) {}
	Vec2I64(const int64_t x, const int64_t y) : X(x), Y(y) {}
	Vec2I64(const Vec2I64& other) { X = other.X; Y = other.Y; }
	Vec2I64(Vec2I64&& other) noexcept { X = other.X; Y = other.Y; }

	~Vec2I64() = default;

	// Assignment
	Vec2I64& operator=(const Vec2I64& other) = default;
	Vec2I64& operator=(Vec2I64&& other) = default;

	// Sum & Subtraction
	Vec2I64 operator+(const Vec2I64& other) const { return { X + other.X, Y + other.Y }; }
	Vec2I64 operator-(const Vec2I64& other) const { return { X - other.X, Y - other.Y }; }
	Vec2I64 operator+=(const Vec2I64& other) { X += other.X; Y += other.Y; return *this; }
	Vec2I64 operator-=(const Vec2I64& other) { X -= other.X; Y -= other.Y; return *this; }

	// Multiplication & Division
	Vec2I64 operator*(const int64_t scalar) const { return { X * scalar, Y * scalar }; }
	Vec2I64 operator*=(const int64_t scalar) { X *= scalar; Y *= scalar; return *this; }
	Vec2I64 operator/(const int64_t scalar) const { return { X / scalar, Y / scalar }; }
	Vec2I64 operator/=(const int64_t scalar) { X /= scalar; Y /= scalar; return *this; }

	// Conversion
	operator Vec3I64() const;
};

struct Vec3I64
{
	int64_t X;
	int64_t Y;
	int64_t Z;

	// Constructors & destructor
	Vec3I64() : X(0), Y(0), Z(0) {}
	Vec3I64(const int64_t x, const int64_t y, const int64_t z) : X(x), Y(y), Z(z) {}
	Vec3I64(const Vec3I64& other) { X = other.X; Y = other.Y; Z = other.Z; }
	Vec3I64(Vec3I64&& other) noexcept { X = other.X; Y = other.Y; Z = other.Z; }

	~Vec3I64() = default;

	// Assignment
	Vec3I64& operator=(const Vec3I64& other) = default;
	Vec3I64& operator=(Vec3I64&& other) = default;

	// Sum & Subtraction
	Vec3I64 operator+(const Vec3I64& other) const { return { X + other.X, Y + other.Y, Z + other.Z }; }
	Vec3I64 operator-(const Vec3I64& other) const { return { X - other.X, Y - other.Y, Z - other.Z }; }
	Vec3I64 operator+=(const Vec3I64& other) { X += other.X; Y += other.Y; Z += other.Z; return *this; }
	Vec3I64 operator-=(const Vec3I64& other) { X -= other.X; Y -= other.Y; Z -= other.Z; return *this; }

	// Multiplication & Division
	Vec3I64 operator*(const int64_t scalar) const { return { X * scalar, Y * scalar, Z * scalar }; }
	Vec3I64 operator*=(const int64_t scalar) { X *= scalar; Y *= scalar; Z *= scalar; return *this; }
	Vec3I64 operator/(const int64_t scalar) const { return { X / scalar, Y / scalar, Z / scalar }; }
	Vec3I64 operator/=(const int64_t scalar) { X /= scalar; Y /= scalar; Z /= scalar; return *this; }

	// Conversion
	operator Vec2I64() const { return { X,Y }; }
};

struct Vec2F
{
	float X;
	float Y;

	// Constructors & destructor
	Vec2F() : X(0), Y(0) { }
	Vec2F(const float x, const float y) : X(x), Y(y) { }
	Vec2F(const Vec2F& other) { X = other.X; Y = other.Y; }
	Vec2F(Vec2F&& other) noexcept { X = other.X; Y = other.Y; }

	~Vec2F() = default;

	// Assignment
	Vec2F& operator=(const Vec2F& other) = default;
	Vec2F& operator=(Vec2F&& other) = default;

	// Sum & Subtraction
	Vec2F operator+(const Vec2F& other) const { return { X + other.X, Y + other.Y }; }
	Vec2F operator-(const Vec2F& other) const { return { X - other.X, Y - other.Y }; }
	Vec2F operator+=(const Vec2F& other) { X += other.X; Y += other.Y; return *this; }
	Vec2F operator-=(const Vec2F& other) { X -= other.X; Y -= other.Y; return *this; }

	// Multiplication & Division
	Vec2F operator*(const float scalar) const { return { X * scalar, Y * scalar }; }
	Vec2F operator*=(const float scalar) { X *= scalar; Y *= scalar; return *this; }
	Vec2F operator/(const float scalar) const { return { X / scalar, Y / scalar }; }
	Vec2F operator/=(const float scalar) { X /= scalar; Y /= scalar; return *this; }

	// Conversion
	operator Vec3F() const;
	operator glm::vec2() const { return { X, Y }; }
};

struct Vec3F
{
	float X;
	float Y;
	float Z;

	// Constructors & destructor
	Vec3F() : X(0), Y(0), Z(0) { }
	Vec3F(const float x, const float y, const float z) : X(x), Y(y), Z(z) { }
	Vec3F(const Vec3F& other) { X = other.X; Y = other.Y; Z = other.Z; }
	Vec3F(Vec3F&& other) noexcept { X = other.X; Y = other.Y; Z = other.Z; }

	~Vec3F() = default;

	// Assignment
	Vec3F& operator=(const Vec3F& other) = default;
	Vec3F& operator=(Vec3F&& other) = default;

	// Sum & Subtraction
	Vec3F operator+(const Vec3F& other) const { return { X + other.X, Y + other.Y, Z + other.Z }; }
	Vec3F operator-(const Vec3F& other) const { return { X - other.X, Y - other.Y, Z - other.Z }; }
	Vec3F operator+=(const Vec3F& other) { X += other.X; Y += other.Y; Z += other.Z; return *this; }
	Vec3F operator-=(const Vec3F& other) { X -= other.X; Y -= other.Y; Z -= other.Z; return *this; }

	// Multiplication & Division
	Vec3F operator*(const float scalar) const { return { X * scalar, Y * scalar, Z * scalar }; }
	Vec3F operator*=(const float scalar) { X *= scalar; Y *= scalar; Z *= scalar; return *this; }
	Vec3F operator/(const float scalar) const { return { X / scalar, Y / scalar, Z / scalar }; }
	Vec3F operator/=(const float scalar) { X /= scalar; Y /= scalar; Z /= scalar; return *this; }

	// Conversion
	operator Vec2F() const { return { X,Y }; }
	operator glm::vec3() const { return { X, Y, Z }; }
};

struct Vec4F
{
	float X;
	float Y;
	float Z;
	float W;

	// Constructors & destructor
	Vec4F() : X(0), Y(0), Z(0), W(0) {}
	Vec4F(const float x, const float y, const float z, const float w) : X(x), Y(y), Z(z), W(w) {}
	Vec4F(const Vec4F& other) { X = other.X; Y = other.Y; Z = other.Z; W = other.W; }
	Vec4F(Vec4F&& other) noexcept { X = other.X; Y = other.Y; Z = other.Z; W = other.W; }

	~Vec4F() = default;

	// Assignment
	Vec4F& operator=(const Vec4F& other) = default;
	Vec4F& operator=(Vec4F&& other) = default;

	// Sum & Subtraction
	Vec4F operator+(const Vec4F& other) const { return { X + other.X, Y + other.Y, Z + other.Z, W + other.W }; }
	Vec4F operator-(const Vec4F& other) const { return { X - other.X, Y - other.Y, Z - other.Z, W - other.W }; }
	Vec4F operator+=(const Vec4F& other) { X += other.X; Y += other.Y; Z += other.Z; W += other.W; return *this; }
	Vec4F operator-=(const Vec4F& other) { X -= other.X; Y -= other.Y; Z -= other.Z; W -= other.W; return *this; }

	// Multiplication & Division
	Vec4F operator*(const float scalar) const { return { X * scalar, Y * scalar, Z * scalar, W * scalar }; }
	Vec4F operator*=(const float scalar) { X *= scalar; Y *= scalar; Z *= scalar; W *= scalar; return *this; }
	Vec4F operator/(const float scalar) const { return { X / scalar, Y / scalar, Z / scalar, W / scalar }; }
	Vec4F operator/=(const float scalar) { X /= scalar; Y /= scalar; Z /= scalar; W /= scalar; return *this; }

	// Conversion
	operator Vec2F() const { return { X,Y }; }
	operator Vec3F() const { return { X,Y,Z }; }
	operator glm::vec4() const { return { X, Y, Z, W }; }
};