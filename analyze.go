package main

//get packages
import (
	"image"
	"image/color"
	"io"
)

//make histogram?
func analyze(imr io.Reader, angles []float64, line []Coord, r int) (*image.RGBA, map[float64]int, error) {
	anglesOf, angleCounts, hood := angleMaskGen(angles, line)
	hist := make(map[float64]int)

	img, imgStats, err := imageRead(imr)
	if err != nil {
		return nil, hist, err
	}
	//Thresholds
	thresh := getThresh(imgStats)
	//puts image out
	imgOut := image.NewRGBA(image.Rect(0, 0, len(img), len(img[0])))

	for x := int16(r); x < int16(len(img)-r); x++ {
		for y := int16(r); y < int16(len(img[x])-r); y++ {
			if img[x][y] < thresh {
				// skip this pixel because it is "empty"
				imgOut.Set(int(x), int(y), color.RGBA{0, 0, 0, 255})
				continue
			}
			angleTotals := make(map[float64]uint32)
			for _, a := range angles {
				angleTotals[a] = 0
			}
			for _, p := range hood {
				for _, a := range anglesOf[p] {
					angleTotals[a] += img[x+p.x][y+p.y]
				}
			}
			best := bestAngle(angleTotals, angleCounts)
			hist[best]++
			imgOut.Set(int(x), int(y), colorScale(best))
		}
	}

	return imgOut, hist, nil
}

func colorScale(angle float64) color.RGBA {
	alpha := uint8(255)
	scale := 255.0 / 30.0

	if angle > 90.0 || angle < -90.0 {
		return color.RGBA{255, 255, 255, alpha}
	}
	if angle > 60.0 {
		val := uint8(scale * (90.0 - angle))
		return color.RGBA{255, val, 0, alpha}
	}
	if angle > 30.0 {
		val := uint8(scale * (angle - 30.0))
		return color.RGBA{val, 255, 0, alpha}
	}
	if angle > 0.0 {
		val := uint8(scale * (30.0 - angle))
		return color.RGBA{0, 255, val, alpha}
	}
	if angle > -30.0 {
		val := uint8(scale * (angle - -30.0))
		return color.RGBA{0, val, 255, alpha}
	}
	if angle > -60.0 {
		val := uint8(scale * (-30.0 - angle))
		return color.RGBA{val, 0, 255, alpha}
	}
	if angle > -90.0 {
		val := uint8(scale * (angle - -90.0))
		return color.RGBA{255, 0, val, alpha}
	}

	return color.RGBA{255, 255, 255, alpha}
}

func bestAngle(angleTotals map[float64]uint32, angleCounts map[float64]uint8) float64 {
	bestAngle := -180.0
	largestTotal := 0.0
	for a, tot := range angleTotals {
		metric := float64(tot) / float64(angleCounts[a])
		if metric > largestTotal {
			largestTotal = metric
			bestAngle = a
		}
	}
	return bestAngle
}

func getThresh(s stats) uint32 {
	//return uint32(s.average)
	//return uint32(s.average / 2.0)
	//if ans := uint32(s.average - s.stddev); ans > 100 { return ans }
	//if ans := uint32(s.average - 0.5 * s.stddev); ans > 100 { return ans }
	if ans := uint32(s.average + 0.5*s.stddev); ans > 100 {
		return ans
	}
	return 100
}
