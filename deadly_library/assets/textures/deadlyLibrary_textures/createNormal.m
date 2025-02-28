function createNormal(height, sigma)
    col = im2double(imread('door_depth.jpg'));
    s = size(col);
    gaussed = imgaussfilt(col, sigma);
    dx = -imfilter(gaussed, [-1 1]);
    sx = dx*0.5+0.5;
    dy = imfilter(gaussed, [-1; 1]);
    sy = dy*0.5+0.5;
    res = cat(3, dx, dy, height*ones(s));
    l = sqrt(res(:,:,1).^2+res(:,:,2).^2+res(:,:,3).^2);
    res(:,:,1)=res(:,:,1)./l;
    res(:,:,1)=res(:,:,1)./l;
    res(:,:,2)=res(:,:,2)./l;
    res(:,:,3)=res(:,:,3)./l;
    
    respacked = res*0.5+0.5;
    imshow(respacked);
    imwrite(respacked, 'door_normals.jpg');
end