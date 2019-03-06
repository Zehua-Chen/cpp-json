class Token:

    def __init__(self, token_type, data=None):
        self.token_type = token_type
        self.data = data

    def __str__(self):

        if self.data == None:
            return self.token_type
        else:
            return "{}: {}".format(self.token_type, self.data)
