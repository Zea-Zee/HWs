import gspread
import asyncio
import datetime
# import smtplib
# from email.mime.text import MIMEText
# from oauth2client.service_account import ServiceAccountCredentials

COLORS = {
    'RED': '\033[91m',
    'GREEN': '\033[92m',
    'YELLOW': '\033[93m',
    'BLUE': '\033[94m',
    'MAGENTA': '\033[95m',
    'CYAN': '\033[96m',
    'WHITE': '\033[97m',
    'RESET': '\033[0m'
}

MAX_ORDERS = 2

gc = gspread.oauth(
    credentials_filename='./client_secret_154894298239-qer4lfqqn8gf7losmjbpdg1ll0316ou2.apps.googleusercontent.com.json',
)

# scope = ['https://spreadsheets.google.com/feeds', 'https://www.googleapis.com/auth/drive']
# credentials = ServiceAccountCredentials.from_json_keyfile_name('./client_secret_154894298239-qer4lfqqn8gf7losmjbpdg1ll0316ou2.apps.googleusercontent.com.json', scope)
#
# gc = gspread.authorize(credentials)
sheet = gc.open_by_url("https://docs.google.com/spreadsheets/d/1_tRnKYp2NGKgjhU5gPXCXA0gxrPR5vFu9vdwGlzwKZw")
machine_1 = sheet.get_worksheet(4)  # Use 0 for the first sheet, 1 for the second, and so on.
machine_2 = sheet.get_worksheet(5)  # Use 0 for the first sheet, 1 for the second, and so on.

records = []

def make_records(machine=1, wanted_count=2):
    num_of_records = 0
    # names_list_1 = ["J18"]
    # rooms_list_1 = ["K18"]
    names_list_1 = ["N12", "P12", "J12", "L12", "N12", "P12", "R12"]
    rooms_list_1 = ["O12", "Q12", "K12", "M12", "O12", "Q12", "S12"]
    names_list_2 = ["J12", "L12", "N12", "P12", "R12"]
    rooms_list_2 = ["K12", "M12", "O12", "Q12", "S12"]
    time_bound = {"F": 10, "H": 12, "J": 14, "L": 16, "N": 18, "P": 20, "R": 22}

    name_cell = names_list_1.pop(0)
    room_cell = rooms_list_1.pop(0)

    while num_of_records < MAX_ORDERS and wanted_count > 0 and machine == 1:
        if not machine_1.acell(name_cell).value and not machine_1.acell(room_cell).value:
            machine_1.update(name_cell, 'Красильников Сергей')
            machine_1.update(room_cell, '215м')

            wanted_count -= 1
            num_of_records += 1
            records.append({"day": int(name_cell[1:]) - 11, "time": time_bound[name_cell[0]]})
        if len(names_list_1) > 0:
            name_cell = names_list_1.pop(0)
            room_cell = rooms_list_1.pop(0)
        else:
                
            break

    while num_of_records < MAX_ORDERS and wanted_count > 0 and machine == 2:
        if not machine_2.acell(name_cell).value and not machine_2.acell(room_cell).value:
            machine_2.update(name_cell, 'Красильников Сергей')
            machine_2.update(room_cell, '215м')

            wanted_count -= 1
            num_of_records += 1
            records.append({"day": int(name_cell[1:]) - 11, "time": time_bound[name_cell[0]]})
        if len(names_list_1) > 0:
            name_cell = names_list_2.pop(0)
            room_cell = rooms_list_2.pop(0)
        else:
            break
    #
    print(COLORS["GREEN"] + "num of success recors:", num_of_records)
    for i in range(len(records)):
        print(COLORS["MAGENTA"] + "day:", records[i]["day"], COLORS["BLUE"], "time:", records[i]["time"])
    return num_of_records

async def wait_washing():
    recs = 0
    flag_1 = False
    flag_2 = False
    while True:
        print(COLORS["YELLOW"] + "checking is reset washings", datetime.datetime.now())
        if not flag_1:
            empty_num_1 = 0
            if not machine_1.acell("R14").value: empty_num_1 += 1
            if not machine_1.acell("P14").value: empty_num_1 += 1
            if not machine_1.acell("R12").value: empty_num_1 += 1
            if not machine_1.acell("P12").value: empty_num_1 += 1
            if not machine_1.acell("H17").value: empty_num_1 += 1
            if not machine_1.acell("J17").value: empty_num_1 += 1
            if not machine_1.acell("L17").value: empty_num_1 += 1
            if empty_num_1 > 3 and not flag_1:
                print('\a')
                recs += make_records(1, 2)
                print(COLORS["CYAN"] + "num of succed records is", recs)
                recf = open("./records.txt", "w");
                recf.write(str(records))
                recf.close()
                if recs > 2:
                    return
        if not flag_2:
                empty_num_2 = 0
                if not machine_2.acell("R14").value: empty_num_2 += 1
                if not machine_2.acell("P14").value: empty_num_2 += 1
                if not machine_2.acell("R12").value: empty_num_2 += 1
                if not machine_2.acell("P12").value: empty_num_2 += 1
                if not machine_2.acell("H17").value: empty_num_2 += 1
                if not machine_2.acell("J17").value: empty_num_2 += 1
                if not machine_2.acell("L17").value: empty_num_2 += 1
                if empty_num_2 > 3:
                    print('\a')
                    recs = make_records(2, 2)
                    print(COLORS["CYAN"] + "num of succed records is", recs)
                    recf = open("./records.txt", "w");
                    recf.write(str(records))
                    recf.close()
                    if recs > 2:
                        return
        if flag_1 and flag_2 or recs == MAX_ORDERS:
            return
        await asyncio.sleep(15)

asyncio.run(wait_washing())
gc.session.close()
exit(0)