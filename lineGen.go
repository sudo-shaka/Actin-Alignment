package main

func flatGen(radius, fatness int) []Coord {
	line := make([]Coord, 0, radius*(1+2*(fatness-1)))

	// Create the outermost points separately. Otherwise some points
	// might be further away than radius units from the center.
	// Very fat short lines might still have an issue here.
	line = append(line, Coord{-int16(radius), 0})
	line = append(line, Coord{int16(radius), 0})

	for i := -radius + 1; i < radius; i++ {
		line = append(line, Coord{int16(i), 0})
		for d := 1; d < fatness; d++ {
			line = append(line, Coord{int16(i), int16(d)})
			line = append(line, Coord{int16(i), -int16(d)})
		}
	}
	return line
}
