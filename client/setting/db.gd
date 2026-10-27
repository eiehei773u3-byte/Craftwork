class_name Settings
extends Node

const SAVE_PATH = "user://setting.cfg"
const DEFAULT_PATH = "res://setting/setting_default.cfg"

var config := ConfigFile.new()

func _init() -> void:
	_load()

# Загрузка конфигурации с проверкой и генерацией случайных значений
func _load() -> void:
	var err = config.load(SAVE_PATH)
	if err != OK:
		print("Пользовательский файл не найден. Создание из дефолтного шаблона...")
		_generate_default_with_randoms()
	else:
		print("Настройки успешно загружены из: ", SAVE_PATH)

# Генерация файла из дефолтного с обработкой макросов user{randomN}
func _generate_default_with_randoms() -> void:
	var file = FileAccess.open(DEFAULT_PATH, FileAccess.READ)
	if file == null:
		push_error("Не удалось открыть дефолтный файл: " + DEFAULT_PATH)
		return
	
	var content = file.get_as_text()
	file.close()
	
	var regex = RegEx.new()
	regex.compile(r"\{random(\d+)\}")
	
	# Ищем все совпадения в тексте
	var results = regex.search_all(content)
	
	# Идем с конца, чтобы замены не сдвигали индексы для следующих совпадений
	results.reverse()
	for res in results:
		var length = res.get_string(1).to_int()
		var random_num = str(_generate_random_number(length))
		
		# Заменяем найденный кусок (например, user{random4}) на случайное число
		var start = res.get_start(0)
		var end = res.get_end(0)
		content = content.substr(0, start) + random_num + content.substr(end)
	
	# Временно сохраняем обработанный текст
	var temp_path = "user://temp_setting.cfg"
	var temp_file = FileAccess.open(temp_path, FileAccess.WRITE)
	temp_file.store_string(content)
	temp_file.close()
	
	# Загружаем через конфиг и сохраняем в итоговый SAVE_PATH
	config.load(temp_path)
	config.save(SAVE_PATH)
	
	# Удаляем временный файл
	DirAccess.remove_absolute(ProjectSettings.globalize_path(temp_path))
	print("Дефолтные настройки сгенерированы и сохранены в: ", SAVE_PATH)

# Генерация случайного числа заданной длины (например, для 4 -> от 1000 до 9999)
func _generate_random_number(length: int) -> int:
	var min_val = int(pow(10, length - 1))
	var max_val = int(pow(10, length) - 1)
	if length == 1:
		min_val = 0
		max_val = 9
	return randi_range(min_val, max_val)

# Получение значения (категория, ключ, дефолт)
# Пример: settings.get_val("audio", "master_volume", 100)
func get_val(section: String, key: String, default_value = null):
	return config.get_value(section, key, default_value)

# Установка значения и автоматическое сохранение
# Пример: settings.set_val("audio", "master_volume", 80)
func set_val(section: String, key: String, value) -> void:
	config.set_value(section, key, value)
	config.save(SAVE_PATH)

# Сброс настроек (при cfg == 1 пересоздает файл заново из дефолтного с рандомом)
func reset(cfg: int) -> void:
	if cfg == 1:
		_generate_default_with_randoms()
		print("Настройки успешно сброшены к заводским.")
