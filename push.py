#!/usr/bin/env python3
import subprocess
import sys

def run_cmd(command):
    """Выполняет команду в терминале и выводит результат"""
    try:
        result = subprocess.run(
            command, 
            shell=True, 
            check=True, 
            text=True, 
            stdout=subprocess.PIPE, 
            stderr=subprocess.PIPE
        )
        if result.stdout.strip():
            print(result.stdout.strip())
        return True
    except subprocess.CalledProcessError as e:
        print(f"Ошибка при выполнении: {command}\n{e.stderr.strip()}", file=sys.stderr)
        return False

def git_auto_push():
    # 1. Получаем сообщение для коммита от пользователя
    commit_msg = input("Введите описание изменений (или Enter для дефолтного): ").strip()
    if not commit_msg:
        commit_msg = "Update project files"

    print("\n[1/3] Индексация файлов (git add .)...")
    if not run_cmd("git add ."):
        return

    print("\n[2/3] Создание коммита (git commit)...")
    # Проверяем, есть ли вообще что коммитить
    status = subprocess.run("git status --porcelain", shell=True, capture_output=True, text=True)
    if not status.stdout.strip():
        print("Нет новых изменений для коммита.")
    else:
        run_cmd(f'git commit -m "{commit_msg}"')

    print("\n[3/3] Отправка на GitHub (git push)...")
    if run_cmd("git push"):
        print("\n✅ Всё успешно отправлено на GitHub!")
    else:
        print("\n⚠️ Обычный push не сработал. Пробуем с привязкой ветки...")
        run_cmd("git push -u origin main")

if __name__ == "__main__":
    git_auto_push()
