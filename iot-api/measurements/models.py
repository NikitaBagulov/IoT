
from sqlalchemy import Column, Integer, Float, String, DateTime
from sqlalchemy.sql import func
from database import Base

class Measurement(Base):
    __tablename__ = "measurements"

    id = Column(Integer, primary_key=True, index=True)
    value = Column(Float)
    location = Column(String)
    device_id = Column(String)
    timestamp = Column(DateTime, default=func.now())

