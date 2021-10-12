package main

import (
	"image"
	_ "image/gif"
	_ "image/jpeg"
	_ "image/png"
	"io"
	"math"
)

type stats struct {
	count   uint64
	average float64
	stddev  float64
}

// Returns a 2d slice of the intensities of the image at the given path,
// as well as some basic statistical data about the image.
func imageRead(imreader io.Reader) ([][]uint32, stats, error) {
	s := stats{}
	stdDevPart := 0.0

	img, _, err := image.Decode(imreader)
	if err != nil {
		return nil, s, err
	}

	bnd := img.Bounds()
	xrange := bnd.Max.X - bnd.Min.X
	yrange := bnd.Max.Y - bnd.Min.Y

	arr := make([][]uint32, xrange, xrange)
	for i := bnd.Min.X; i < bnd.Max.X; i++ {
		arr[i] = make([]uint32, yrange, yrange)
		for j := bnd.Min.Y; j < bnd.Max.Y; j++ {
			r, g, b, _ := img.At(i, j).RGBA()
			val := r + g + b
			arr[i][j] = val

			// These formulae adapted from Knuth's TAOCP Vol 2, sect 4.2.2:
			s.count++
			oldAve := s.average
			s.average += (float64(val) - s.average) / float64(s.count)
			stdDevPart += (float64(val) - oldAve) * (float64(val) - s.average)
		}
	}
	// Again, from Knuth:
	s.stddev = math.Sqrt(stdDevPart / (float64(s.count) - 1))

	return arr, s, nil
}
