import requests
import unittest

class TestGetOgliveListMethods(unittest.TestCase):

    def setUp(self):
        self.url = 'http://localhost:8888/oglive/list'
        self.headers = {'Authorization' : '07b3bfe728954619b58f0107ad73acc1'}

    def test_get(self):
        returned = requests.get(self.url, headers=self.headers)
        self.assertEqual(returned.status_code, 200)

    def test_post(self):
        returned = requests.post(self.url, headers=self.headers)
        self.assertEqual(returned.status_code, 405)

if __name__ == '__main__':
    unittest.main()
