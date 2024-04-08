%% Matlab function which does approximatly the same thing as the go-lang code


function [angles,counts,entropy,newimage] = AnalyizeActin(IMAGE)
  mask = imbinarize(imgaussfilt(IMAGE,0.5));
  newimage = zeros(size(mask));

  linelength = ceil(length(IMAGE)/20);
  numangles = 11;

  angles = linspace(0,pi,numangles+1);
  angles = angles(1:end-1);
  counts= zeros(1,numangles);
  anglechange = angles(2);

  lines = zeros(2,linelength,numangles);
  lines(1,:,1) = linspace(-linelength/2,linelength/2,linelength);

  c = cos(anglechange);
  s = sin(anglechange);
  for ii = 2:length(counts)
    lines(1,:,ii) = c.*lines(1,:,ii-1) - s.*lines(2,:,ii-1);
    lines(2,:,ii) = s.*lines(1,:,ii-1) + c.*lines(2,:,ii-1);
  end

  lines = round(lines);
  tmplines = lines;
  c = zeros(1,numangles);
  for x = 1:length(IMAGE)
    for y = 1:length(IMAGE)
      if mask(x,y)
        tmplines(1,:,:) = lines(1,:,:) + x;
        tmplines(2,:,:) = lines(2,:,:) + y;

        for ii = 1:numangles
          tmplines(tmplines < 1) = 1;
          tmplines(tmplines > length(IMAGE)) = length(IMAGE);

          submask = sub2ind(size(mask),tmplines(1,:,ii),tmplines(2,:,ii));
          c(ii) = mean(mask(submask));
        end

        [m,midx] = max(c);
        newimage(x,y) = abs(rad2deg(angles(midx))-90);
        counts(midx) = counts(midx) + 1;
      end
    end
  end
  newimage = imadjust(uint8(newimage));

  normcounts = counts./sum(counts);
  logx = @(x) log(x)/log(length(angles));
  entropy = -sum(normcounts./logx(normcounts));
end
