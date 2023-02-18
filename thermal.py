# https://makersportal.com/blog/2020/6/8/high-resolution-thermal-camera-with-raspberry-pi-and-mlx90640
# a simple but effective code for determining the limit temperature in the picture obtained from the thermal sensor from Malex mlx90640

# termal & Rpi lib
import time,board,busio
import adafruit_mlx90640

i2c = busio.I2C(board.SCL, board.SDA, frequency=400000) # setup I2C
mlx = adafruit_mlx90640.MLX90640(i2c) # begin MLX90640 with I2C comm
mlx.refresh_rate = adafruit_mlx90640.RefreshRate.REFRESH_16_HZ # set refresh rate
mlx_shape = (24,32) # mlx90640 shape

frame = np.zeros(mlx_shape[0]*mlx_shape[1]) # 768 pts

def process_mlx90640(mlx):
    flag = 0
    mlx.getFrame(frame) # read mlx90640
    for number in frame.flat:
        if number > 100:
            flag = 1
            break
    return flag

flag = process_mlx90640(mlx)
print("Flag:", flag)
