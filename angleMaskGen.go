package main

import ("math"
"fmt")

type Coord struct {
	x, y int16
}

// Determines which points are covered by which angles of the given "line"
// Also determines how many points are covered by each rotation of the "line"
//
// Usage: anglesOf(point) -> slice of angles which cover this point.
// 	angleCounts(angle) -> number of points covered by the "line" at this angle.
func angleMaskGen(angles []float64, line []Coord) (map[Coord][]float64, map[float64]uint8, []Coord) {
	anglesOf := make(map[Coord][]float64)
	angleCounts := make(map[float64]uint8)
	neighborSet := make(map[Coord]bool)
  
	for _, a := range angles {
		for _, p := range line {
			rp := rotate(p, a)
			neighborSet[rp] = true
			if _, inside := anglesOf[rp]; !inside {
        //fmt.Printf("%#v : %#v\n",inside,anglesOf[rp])
				anglesOf[rp] = []float64{a}
				angleCounts[a]++
			} else if notInSlice(anglesOf[rp], a) {
        fmt.Printf("%#v : %#v\n",a,anglesOf[rp])
				anglesOf[rp] = append(anglesOf[rp], a)
				angleCounts[a]++
			}
      //fmt.Printf("%#v\n",anglesOf[rp])
		}
	}

	neighborSlice := make([]Coord, 0, len(line)*len(angles))
	for n, _ := range neighborSet {
		neighborSlice = append(neighborSlice, n)
	}
	return anglesOf, angleCounts, neighborSlice
}

func rotate(c Coord, a float64) Coord {
	sin, cos := math.Sincos(a * math.Pi / 180.0)
	return Coord{
		x: int16(math.Round(cos*float64(c.x) - sin*float64(c.y))),
		y: int16(math.Round(sin*float64(c.x) + cos*float64(c.y))),
	}
}

func notInSlice(slice []float64, val float64) bool {
	for _, v := range slice {
		if v == val {
			return false
		}
	}
	return true
}
