extends Resource
class_name ConsoleCommands

func register_commands() -> void:
	Console.add_command("save_game", _on_save_game, ["slot"], 0, "Save game to slot")
	Console.add_command_autocomplete_list("save_game", PackedStringArray(["default", "slot1", "slot2"]))

	Console.add_command("save_delete", _on_save_delete, ["target"], 1, "Delete save or reset settings")
	Console.add_command_autocomplete_list("save_delete", PackedStringArray(["setting", "slot1", "slot2"]))


func _on_save_game(slot: String) -> void:
	slot = slot.strip_edges()
	if slot == "":
		slot = "default"
		
	if Db and Db.has_method("save_game"):
		Db.save_game(slot)
		Console.print_info("Saved to slot: " + slot)
	else:
		Console.print_error("Db.save_game() not found")


func _on_save_delete(target: String) -> void:
	target = target.strip_edges().to_lower()
	
	if target == "":
		Console.print_error("Usage: save_delete <setting|slot1|slot2>")
		return
	
	if target == "setting":
		if Db and Db.has_method("reset"):
			Db.reset(1)
			Console.print_info("Settings reset")
		else:
			Console.print_error("Db.reset() not found")
	else:
		if Db and Db.has_method("delete_save"):
			Db.delete_save(target)
			Console.print_info("Deleted: " + target)
		else:
			Console.print_error("Db.delete_save() not found")
