from bind_template.scalar import ProcessScalar


def test_ProcessScalar_add():
    obj = ProcessScalar()
    assert obj.add(1, 2) == 3
    assert obj.add(-1, 1) == 0


def test_ProcessScalar_multiply():
    obj = ProcessScalar()
    assert obj.multiply(2, 3) == 6
    assert obj.multiply(-1, 5) == -5
    