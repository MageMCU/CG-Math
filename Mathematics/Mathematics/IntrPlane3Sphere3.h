// David Eberly, Geometric Tools, Redmond WA 98052
// Copyright (c) 1998-2023
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt
// https://www.geometrictools.com/License/Boost/LICENSE_1_0.txt
// Version: 6.0.2022.01.06

#pragma once

#include "FIQuery.h"
#include "TIQuery.h"
#include "DistPointHyperplane.h"
#include "Hypersphere.h"
#include "Circle3.h"

namespace gte
{
    template <typename T>
    class TIQuery<T, Plane3<T>, Sphere3<T>>
    {
    public:
        struct Result
        {
            Result()
                :
                intersect(false)
            {
            }

            bool intersect;
        };

        Result operator()(Plane3<T> const& plane, Sphere3<T> const& sphere)
        {
            Result result{};
            DCPQuery<T, Vector3<T>, Plane3<T>> ppQuery;
            auto ppResult = ppQuery(sphere.center, plane);
            result.intersect = (ppResult.distance <= sphere.radius);
            return result;
        }
    };

    template <typename T>
    class FIQuery<T, Plane3<T>, Sphere3<T>>
    {
    public:
        struct Result
        {
            Result()
                :
                intersect(false),
                isCircle(false),
                circle(Vector3<T>::Zero(), Vector3<T>::Zero(), (T)0),
                point(Vector3<T>::Zero())
            {
            }

            bool intersect;

            // If 'intersect' is true, the intersection is either a point or a
            // circle.  When 'isCircle' is true, 'circle' is valid.  When
            // 'isCircle' is false, 'point' is valid.
            bool isCircle;
            Circle3<T> circle;
            Vector3<T> point;
        };

        Result operator()(Plane3<T> const& plane, Sphere3<T> const& sphere)
        {
            Result result{};
            DCPQuery<T, Vector3<T>, Plane3<T>> ppQuery;
            auto ppResult = ppQuery(sphere.center, plane);
            if (ppResult.distance < sphere.radius)
            {
                result.intersect = true;
                result.isCircle = true;
                result.circle.center = sphere.center - ppResult.signedDistance * plane.normal;
                result.circle.normal = plane.normal;

                // The sum and diff are both positive numbers.
                T sum = sphere.radius + ppResult.distance;
                T dif = sphere.radius - ppResult.distance;

                // arg = sqr(sphere.radius) - sqr(ppResult.distance)
                T arg = sum * dif;

                result.circle.radius = std::sqrt(arg);
                return result;
            }
            else if (ppResult.distance == sphere.radius)
            {
                result.intersect = true;
                result.isCircle = false;
                result.point = sphere.center - ppResult.signedDistance * plane.normal;
                return result;
            }
            else
            {
                result.intersect = false;
                return result;
            }
        }
    };
}