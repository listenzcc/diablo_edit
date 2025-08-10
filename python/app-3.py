from nicegui import ui
from typing import List, Dict, Set
from dataclasses import dataclass


@dataclass
class ItemRecord:
    player_name: str
    item_name: str
    quality: int


def parse_item_file(file_path: str) -> List[ItemRecord]:
    records = []
    with open(file_path, 'r', encoding='utf-8') as f:
        current_record = {}
        for line in f:
            line = line.strip()
            if line.startswith('Player Name:'):
                current_record['player_name'] = line.split(': ')[1]
            elif line.startswith('Item Name:'):
                current_record['item_name'] = line.split(': ')[1]
            elif line.startswith('Quality:'):
                current_record['quality'] = int(line.split(': ')[1])
            elif line.startswith('------------------------------'):
                if current_record:
                    records.append(ItemRecord(**current_record))
                    current_record = {}
    return records


class ItemQueryApp:
    def __init__(self, file_path: str):
        self.file_path = file_path
        self.records = parse_item_file(file_path)
        self.available_qualities = sorted({r.quality for r in self.records})
        self.setup_ui()

    def setup_ui(self):
        # 共享的品质多选框
        with ui.card().classes('w-full'):
            ui.label('Quality Filter').classes('text-xl')
            self.quality_checkboxes = {}
            with ui.row().classes('w-full'):
                for quality in self.available_qualities:
                    self.quality_checkboxes[quality] = ui.checkbox(
                        text=str(quality),
                        value=True,
                        on_change=self.update_all_tables
                    )

        # Player to Items query
        with ui.card().classes('w-full'):
            ui.label('Player to Items Query').classes('text-xl')
            self.player_select = ui.select(
                options=sorted({r.player_name for r in self.records}),
                label='Select Player',
                on_change=self.update_player_items
            ).classes('w-full')

            columns = [
                {'name': 'item_name', 'label': 'Item Name', 'field': 'item_name'},
                {'name': 'quality', 'label': 'Quality', 'field': 'quality'}
            ]
            self.player_items_table = ui.table(
                columns=columns, rows=[]).classes('w-full')

        # Item to Players query
        with ui.card().classes('w-full'):
            ui.label('Item to Players Query').classes('text-xl')
            self.item_select = ui.select(
                options=sorted({r.item_name for r in self.records}),
                label='Select Item',
                on_change=self.update_item_players
            ).classes('w-full')

            columns = [
                {'name': 'player_name', 'label': 'Player Name',
                    'field': 'player_name'},
                {'name': 'quality', 'label': 'Quality', 'field': 'quality'}
            ]
            self.item_players_table = ui.table(
                columns=columns, rows=[]).classes('w-full')

        # 初始化时显示数据
        self.update_all_tables()

    def get_selected_qualities(self) -> Set[int]:
        return {q for q, cb in self.quality_checkboxes.items() if cb.value}

    def update_all_tables(self):
        self.update_player_items()
        self.update_item_players()

    def update_player_items(self):
        player_name = self.player_select.value
        selected_qualities = self.get_selected_qualities()

        if not player_name:
            self.player_items_table.rows = []
        else:
            rows = [
                {'item_name': r.item_name, 'quality': r.quality}
                for r in self.records
                if r.player_name == player_name and r.quality in selected_qualities
            ]
            self.player_items_table.rows = rows
        self.player_items_table.update()

    def update_item_players(self):
        item_name = self.item_select.value
        selected_qualities = self.get_selected_qualities()

        if not item_name:
            self.item_players_table.rows = []
        else:
            rows = [
                {'player_name': r.player_name, 'quality': r.quality}
                for r in self.records
                if r.item_name == item_name and r.quality in selected_qualities
            ]
            self.item_players_table.rows = rows
        self.item_players_table.update()


# 使用示例
app = ItemQueryApp('./x64/Release/character_items.txt')

ui.run(title='Diablo II Item Query', port=8080)
