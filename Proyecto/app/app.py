import asyncio
from bleak import BleakClient, BleakGATTCharacteristic
from tkinter import *
import customtkinter
from PIL import Image, ImageTk

FLAG_ON = b"\x01"
FLAG_OFF = b"\x00"

UUID_SERVICE = "6282a2a1-5f99-430e-abda-ba09b255bdd4"
UUID_PROCESAR = "99ee8226-8c9e-4972-b4f5-ea7af1caa0b5"
UUID_MAX_ANGULO = "17cc3d4f-69c7-4e00-a954-0346ecbb22c7"
UUID_MIN_ANGULO = "6a8adb71-4b3b-421f-b5e1-e74bad969122"
UUID_ANGULO = "38b48b19-373f-4425-a790-42df14c08300"
UUID_ESTADO = "c4346fa0-d0ee-4c24-817f-0fab75a7fd23"
UUID_TIEMPO_SUBIDA = "64247fd3-2d0e-461f-ac13-8651d2ba991e"
UUID_TIEMPO_ALTO = "ddc902e8-9d9f-4db6-ba57-76609f6d2adb"

device_address = "18:14:25:D8:88:BD"
device_connected = False

# UI Components
root = None
font_default = None
image_background = None
bt_inicio = None
bar_angulo = None
bar_angulo_max = None
bar_angulo_min = None
frame_bar = None
image_max = None
icon_max = None
image_min = None
icon_min = None
lbl_estado_sensor = None
frame_table = None
lbl_tiempo_subida = None
lbl_tiempo_subida_tiempo = None
lbl_tiempo_arriba = None
lbl_tiempo_arriba_tiempo = None

# Estado
ST_IDLE = 0
ST_RUNNING = 1
state = ST_IDLE
bt_inicio_pressed = False

# Estado sensor
ESTADOS_SENSOR = ["Desconectado", "Parado", "Subiendo", "Arriba", "Bajando"]
state_sensor = 0

angulo = 0


def hsl_to_hex(h, s, l):
    # Normalizar los valores de H, S y L
    h = h % 360
    s = max(0, min(100, s))
    l = max(0, min(100, l))
    
    # Calcular los valores de R, G y B
    c = (1 - abs(2*l/100 - 1)) * s/100
    x = c * (1 - abs((h/60) % 2 - 1))
    m = l/100 - c/2
    
    if 0 <= h < 60:
        r, g, b = c, x, 0
    elif 60 <= h < 120:
        r, g, b = x, c, 0
    elif 120 <= h < 180:
        r, g, b = 0, c, x
    elif 180 <= h < 240:
        r, g, b = 0, x, c
    elif 240 <= h < 300:
        r, g, b = x, 0, c
    else:
        r, g, b = c, 0, x
    
    # Convertir los valores de R, G y B a hexadecimal
    r_hex = int((r + m) * 255)
    g_hex = int((g + m) * 255)
    b_hex = int((b + m) * 255)
    
    return "#{:02x}{:02x}{:02x}".format(r_hex, g_hex, b_hex)


def characteristic_callback(char : BleakGATTCharacteristic, data : bytearray):
    if char.uuid == UUID_ANGULO:
        angulo = int.from_bytes(bytes=data, byteorder='little', signed=True)
        bar_angulo.set(angulo/90.0)
        bar_angulo.configure(progress_color=hsl_to_hex(angulo, 72, 53), fg_color=hsl_to_hex(angulo, 15, 30))
    elif char.uuid == UUID_MAX_ANGULO:
        angulo = int.from_bytes(bytes=data, byteorder='little', signed=True)
        bar_angulo_max.set(angulo/90.0)
        bar_angulo_max.configure(progress_color=hsl_to_hex(angulo, 72, 53), fg_color=hsl_to_hex(angulo, 15, 30))
        icon_max.place_configure(anchor=CENTER, relx=0.5, y=(130-angulo/90.0*240))
        icon_max.configure(fg_color=hsl_to_hex(angulo, 72, 53))
    elif char.uuid == UUID_MIN_ANGULO:
        angulo = int.from_bytes(bytes=data, byteorder='little', signed=True)
        bar_angulo_min.set(angulo/90.0)
        bar_angulo_min.configure(progress_color=hsl_to_hex(angulo, 72, 53), fg_color=hsl_to_hex(angulo, 15, 30))
        icon_min.place_configure(anchor=CENTER, relx=0.5, y=(130-angulo/90.0*240))
        icon_min.configure(fg_color=hsl_to_hex(angulo, 72, 53))
    elif char.uuid == UUID_ESTADO:
        lbl_estado_sensor.configure(text=ESTADOS_SENSOR[int.from_bytes(bytes=data, byteorder='little', signed=True)])
    elif char.uuid == UUID_TIEMPO_ALTO:
        lbl_tiempo_arriba_tiempo.configure(text=str(int.from_bytes(bytes=data, byteorder='little', signed=True))+"s")
    elif char.uuid == UUID_TIEMPO_SUBIDA:
        lbl_tiempo_subida_tiempo.configure(text=str(int.from_bytes(bytes=data, byteorder='little', signed=True))+"s")


def button_event():
    global state
    global bt_inicio_pressed
    if state == ST_IDLE:
        bt_inicio.configure(text="Parar", fg_color="#eeb044", hover_color="#eac88c")
        root.configure(fg_color="#543808")
        state = ST_RUNNING
    else:
        bt_inicio.configure(text="Iniciar", fg_color="#007ea8", hover_color="#6f99a7")
        root.configure(fg_color="#12303B")
        state = ST_IDLE
    bt_inicio_pressed = True


async def main():
    async with BleakClient(device_address) as client:
        global bt_inicio_pressed
        service_movimiento = client.services.get_service(UUID_SERVICE)
        characteristic_procesar = service_movimiento.get_characteristic(UUID_PROCESAR)
        characteristic_max_angulo = service_movimiento.get_characteristic(UUID_MAX_ANGULO)
        characteristic_min_angulo = service_movimiento.get_characteristic(UUID_MIN_ANGULO)
        characteristic_angulo = service_movimiento.get_characteristic(UUID_ANGULO)
        characteristic_estado = service_movimiento.get_characteristic(UUID_ESTADO)
        characteristic_tiempo_subida = service_movimiento.get_characteristic(UUID_TIEMPO_SUBIDA)
        characteristic_tiempo_alto = service_movimiento.get_characteristic(UUID_TIEMPO_ALTO)
        await client.start_notify(characteristic_max_angulo, characteristic_callback)
        await client.start_notify(characteristic_min_angulo, characteristic_callback)
        await client.start_notify(characteristic_angulo, characteristic_callback)
        await client.start_notify(characteristic_estado, characteristic_callback)
        await client.start_notify(characteristic_tiempo_subida, characteristic_callback)
        await client.start_notify(characteristic_tiempo_alto, characteristic_callback)
        # await client.write_gatt_char(characteristic_procesar, FLAG_ON, response=True)

        while True:
            root.update()
            if bt_inicio_pressed:
                if state == ST_IDLE:
                    await client.write_gatt_char(characteristic_procesar, FLAG_OFF, response=True)
                else:
                    await client.write_gatt_char(characteristic_procesar, FLAG_ON, response=True)
                bt_inicio_pressed = False
            await asyncio.sleep(0.01)

    print("Finalizado")


customtkinter.set_appearance_mode("dark")
root = customtkinter.CTk("#12303B")
root.title("PuntillasApp")
root.geometry("450x800")

font_default = customtkinter.CTkFont(family="", size=16)

# image_background = customtkinter.CTkLabel(root, image=customtkinter.CTkImage(Image.open("background.png"), size=(900,1600)), text="",
#                                   fg_color="transparent")
# image_background.place_configure(anchor=CENTER, x=0, y=0)

frame_bar = customtkinter.CTkFrame(master=root, height=500, fg_color="transparent")
frame_bar.place_configure(anchor=N, relx=0.5, rely=0.2)


bar_angulo_min = customtkinter.CTkProgressBar(master=frame_bar, orientation="vertical", width=26, height=260)
bar_angulo_min.pack(side=LEFT, padx=4)
bar_angulo_max = customtkinter.CTkProgressBar(master=frame_bar, orientation="vertical", width=26, height=260)
bar_angulo_max.pack(side=LEFT, padx=4)
bar_angulo = customtkinter.CTkProgressBar(master=frame_bar, orientation="vertical", width=26, height=260)
bar_angulo.pack(side=LEFT, padx=4)

image_max = customtkinter.CTkImage(Image.open("icon_max_white.png"), size=(24,24))
icon_max = customtkinter.CTkLabel(bar_angulo_max, corner_radius=10, image=image_max, text="",
                                  fg_color="transparent")
icon_max.place_configure(anchor=CENTER, relx=0.5, rely=0.5)

image_min = customtkinter.CTkImage(Image.open("icon_min_white.png"), size=(24,24))
icon_min = customtkinter.CTkLabel(bar_angulo_min, corner_radius=10, image=image_min, text="",
                                  fg_color="transparent")
icon_min.place_configure(anchor=CENTER, relx=0.5, rely=0.5)

bt_inicio = customtkinter.CTkButton(master=root, text="Iniciar", command=lambda: button_event(),
                                    width=180, height=36, corner_radius=18, font=font_default)
bt_inicio.place_configure(anchor=S, relx=0.5, rely=0.95)

lbl_estado_sensor = customtkinter.CTkLabel(master=root, text=ESTADOS_SENSOR[0], text_color="white",
                                           font=font_default, fg_color="transparent")
lbl_estado_sensor.place_configure(anchor=N, relx=0.5, y=48)

frame_table = customtkinter.CTkFrame(master=root, height=500, fg_color="transparent")
frame_table.place_configure(anchor=S, relx=0.5, rely=0.7)

lbl_tiempo_subida = customtkinter.CTkLabel(frame_table, text="Tiempo de subida", font=font_default)
lbl_tiempo_subida.grid(column=0, columnspan=6, row=0, rowspan=1, sticky="W", padx=(0, 32))
lbl_tiempo_subida_tiempo = customtkinter.CTkLabel(frame_table, text="--", font=font_default)
lbl_tiempo_subida_tiempo.grid(column=6, columnspan=2, row=0, rowspan=1, sticky="E", padx=(32, 0))

lbl_tiempo_arriba = customtkinter.CTkLabel(frame_table, text="Tiempo de puntillas", font=font_default)
lbl_tiempo_arriba.grid(column=0, columnspan=6, row=1, rowspan=1, sticky="W", padx=(0, 32))
lbl_tiempo_arriba_tiempo = customtkinter.CTkLabel(frame_table, text="--", font=font_default)
lbl_tiempo_arriba_tiempo.grid(column=6, columnspan=2, row=1, rowspan=1, sticky="E", padx=(32, 0))

asyncio.run(main())