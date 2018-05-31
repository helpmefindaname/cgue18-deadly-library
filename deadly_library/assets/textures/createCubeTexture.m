function createCubeTexture(front, back, left, right, top, bottom, out, size)
    front_data = imresize(imread(front), [size size]);
    back_data = imresize(imread(back), [size size]);
    left_data = imresize(imread(left), [size size]);
    right_data = imresize(imread(right), [size size]);
    top_data = imresize(imread(top), [size size]);
    bottom_data = imresize(imread(bottom), [size size]);
    
    out_data = [front_data back_data right_data;
                left_data top_data bottom_data];
            
	imwrite(out_data, out);
end
