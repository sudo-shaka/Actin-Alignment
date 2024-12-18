package main

import (
	"fmt"
	"image/png"
	"os"
	"path/filepath"
	"regexp"
)

func main() {
	angles := anglesEvery(15.0)
	radius := 20
	line := flatGen(radius, 1)

	// Last argument defines what file(s) to work on
	imgPaths := getImagePaths(os.Args[len(os.Args)-1])

	if len(os.Args) <= 1 {
		imgPaths = getImagePaths(".")
	}

	for _, path := range imgPaths {
		fmt.Println("Getting image from ", path)
		imgreader, err := os.Open(path)
		if err != nil {
			fmt.Printf("Unable to read file: %v\n", err)
			return
		}

		imgOut, hist, err := analyze(imgreader, angles, line, radius)
		if err != nil {
			fmt.Printf("Unable to analyze image: %v\n", err)
			return
		}

		histFile, err := os.Create(filepath.Base(path) + "-histogram.csv")
		if err != nil {
			fmt.Printf("Unable to create file: %v\n", err)
			return
		}
		defer histFile.Close()

		total := 0
		histFile.WriteString("Angle,NumOfPixels\n")
		for _, a := range angles {
			histFile.WriteString(fmt.Sprintf("%v\u00B0,%v\n", a, hist[a]))
			total += hist[a]
		}
		histFile.WriteString(fmt.Sprintf("total,%v\n", total))

		imgFile, err := os.Create(filepath.Base(path) + "-out.png")
		if err != nil {
			fmt.Printf("Unable to create file: %v\n", err)
			return
		}
		defer imgFile.Close()

		if err = png.Encode(imgFile, imgOut); err != nil {
			fmt.Printf("Unable to encode image: %v\n", err)
			return
		}
	}

	fmt.Println("Done!")
}

func getImagePaths(arg string) []string {
	info, err := os.Stat(arg)
	if err != nil {
		panic(err)
	}

	if !info.IsDir() {
		return []string{arg}
	}

	files, err := os.ReadDir(arg)
	if err != nil {
		panic(err)
	}

	paths := make([]string, 0)
	for _, file := range files {
		if file.IsDir() {
			continue
		}
		isImg, _ := regexp.Match(`(\.png$)|(\.jpg$)|(\.gif$)|(\.tiff$)|(\.tif$)`, []byte(file.Name()))
		if isImg {
			paths = append(paths, filepath.Join(arg, file.Name()))
		}
	}
	return paths
}

func anglesEvery(delta float64) []float64 {
	a := make([]float64, 0, 180/int(delta))
	for ang := 90.0; ang > -90.0; ang -= delta {
		a = append(a, ang)
	}
	return a
}
