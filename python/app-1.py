from nicegui import ui
from typing import List, Dict
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
        self.setup_ui()

    def setup_ui(self):
        # Player to Items query
        with ui.card().classes('w-full'):
            ui.label('Player to Items Query').classes('text-xl')
            self.player_select = ui.select(
                options=sorted({r.player_name for r in self.records}),
                label='Select Player'
            ).classes('w-full')
            self.min_quality_player = ui.number(
                label='Minimum Quality',
                value=0,
                min=0,
                max=20
            ).classes('w-full')
            ui.button('Query', on_click=self.query_player_items).classes(
                'w-full')

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
                label='Select Item'
            ).classes('w-full')
            self.min_quality_item = ui.number(
                label='Minimum Quality',
                value=0,
                min=0,
                max=20
            ).classes('w-full')
            ui.button('Query', on_click=self.query_item_players).classes(
                'w-full')

            columns = [
                {'name': 'player_name', 'label': 'Player Name',
                    'field': 'player_name'},
                {'name': 'quality', 'label': 'Quality', 'field': 'quality'}
            ]
            self.item_players_table = ui.table(
                columns=columns, rows=[]).classes('w-full')

    def query_player_items(self):
        player_name = self.player_select.value
        min_quality = self.min_quality_player.value
        rows = [
            {'item_name': r.item_name, 'quality': r.quality}
            for r in self.records
            if r.player_name == player_name and r.quality >= min_quality
        ]
        self.player_items_table.rows = rows
        self.player_items_table.update()

    def query_item_players(self):
        item_name = self.item_select.value
        min_quality = self.min_quality_item.value
        rows = [
            {'player_name': r.player_name, 'quality': r.quality}
            for r in self.records
            if r.item_name == item_name and r.quality >= min_quality
        ]
        self.item_players_table.rows = rows
        self.item_players_table.update()


# 使用示例
app = ItemQueryApp('./x64/Release/character_items.txt')

ui.run(title='Diablo II Item Query', port=8080)
